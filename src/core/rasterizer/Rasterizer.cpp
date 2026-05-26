#include "core/rasterizer/Rasterizer.hpp"

#include "utility/MathsUtils.hpp"
#include "utility/FXColor.hpp"
#include "utility/Interpolator.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <functional>

bool Rasterizer::s_EnableFaceCulling = true;  // Face culling enabled by default
bool Rasterizer::s_TwoSidedRenderingEnabled = false;  // Two-sided rendering disabled by default

void Rasterizer::SetFaceCullingEnabled(bool enabled) {
    s_EnableFaceCulling = enabled;
}

bool Rasterizer::IsFaceCullingEnabled() {
    return s_EnableFaceCulling;
}

void Rasterizer::SetTwoSidedRenderingEnabled(bool enabled) {
    s_TwoSidedRenderingEnabled = enabled;
}

bool Rasterizer::IsTwoSidedRenderingEnabled() {
    return s_TwoSidedRenderingEnabled;
}

bool Rasterizer::IsFrontFacing(const Gpu& gpu, const GpuVertex& v1, const GpuVertex& v2, const GpuVertex& v3) {
    // Compute face normal from edge cross product
    FVector3 edge1 = v2.worldPosition - v1.worldPosition;
    FVector3 edge2 = v3.worldPosition - v1.worldPosition;
    FVector3 faceNormal = edge1.Cross(edge2);
    
    // Direction from triangle to camera
    FVector3 directionToCamera = gpu.cameraPosition - v1.worldPosition;
    
    // If normal and direction to camera have positive dot product, triangle faces camera
    return faceNormal.Dot(directionToCamera) >= 0.f;
}


bool Rasterizer::IsPointInTriangleBBox(IVector2 p, IVector2 a, IVector2 b, IVector2 c) {
    // Calculate the area of the triangle formed by the three vertices (a, b, c) and the point (p)
    bool b1 = MathsUtils::Det(p, a, b) < 0;
    bool b2 = MathsUtils::Det(p, b, c) < 0;
    bool b3 = MathsUtils::Det(p, c, a) < 0;
    // if the point is on the same side of all three edges of the triangle, then it is inside the triangle
    // if b1, b2, and b3 are all true or all false, then the point is inside the triangle
    return (b1 == b2) && (b2 == b3);
}

void Rasterizer::RasterizeRow(const Gpu& gpu, int y,
                  const GpuVertex& leftV1, const GpuVertex& leftV2,
                  const GpuVertex& rightV1, const GpuVertex& rightV2,
                  Screen* screen)
{
    // From first point to second point of the left edge and first point to second point of the right edge
    // we interpolate the x coordinate for the current y coordinate
    const IVector2& leftEdgeSP1  = leftV1.screenPosition;
    const IVector2& leftEdgeSP2  = leftV2.screenPosition;
    const IVector2& rightEdgeSP1 = rightV1.screenPosition;
    const IVector2& rightEdgeSP2 = rightV2.screenPosition;

    // We calculate the gradient of the current y coordinate between the two points of the edge 
    // to determine how much to interpolate the x coordinate
    float leftGradientY = 1.f;
    if (leftEdgeSP2.y != leftEdgeSP1.y) 
        // This is the distance of the current y coordinate from the first point of the edge divided 
        // by the total distance between the two points of the edge in the y direction
        // that gives us a value between 0 and 1 that we can use to interpolate the x coordinate 
        // between the two points of the edge
        leftGradientY = static_cast<float>(y - leftEdgeSP1.y) / static_cast<float>(leftEdgeSP2.y - leftEdgeSP1.y);

    // We do the same for the right edge to determine how much to interpolate the x coordinate on the right edge
    float rightGradientY = 1.f;
    if (rightEdgeSP2.y != rightEdgeSP1.y) 
        rightGradientY = static_cast<float>(y - rightEdgeSP1.y) / static_cast<float>(rightEdgeSP2.y - rightEdgeSP1.y);


    // We then interpolate the x coordinate for the current y coordinate 
    //on both the left and right edges using the calculated gradients
    int leftX  = static_cast<int>(Interpolator::Scalar(static_cast<float>(leftEdgeSP1.x),  static_cast<float>(leftEdgeSP2.x),  leftGradientY));
    int rightX = static_cast<int>(Interpolator::Scalar(static_cast<float>(rightEdgeSP1.x), static_cast<float>(rightEdgeSP2.x), rightGradientY));

    // We also interpolate the z coordinate for the current y coordinate on both edges 
    // to determine the depth of the pixels we will be drawing
    // So we don't overrite pixels that are closer to the camera with pixels that are farther away
    float leftZ  = Interpolator::Scalar(leftV1.zPosition,  leftV2.zPosition,  leftGradientY);
    float rightZ = Interpolator::Scalar(rightV1.zPosition, rightV2.zPosition, rightGradientY);

    Color leftColor{}, rightColor{};
    FVector2 leftUV{}, rightUV{};

    if (gpu.Mode == GpuDrawMode::COLOR) {
        leftColor  = Interpolator::ColorLerp(leftV1.color,  leftV2.color,  leftGradientY);
        rightColor = Interpolator::ColorLerp(rightV1.color, rightV2.color, rightGradientY);
    } else if (gpu.Mode == GpuDrawMode::TEXTURE) {
        leftUV  = Interpolator::Vector2(leftV1.uv,  leftV2.uv,  leftGradientY);
        rightUV = Interpolator::Vector2(rightV1.uv, rightV2.uv, rightGradientY);
    }

    FVector3 leftWorldPosition  = Interpolator::Vector3(leftV1.worldPosition,  leftV2.worldPosition,  leftGradientY);
    FVector3 rightWorldPosition = Interpolator::Vector3(rightV1.worldPosition, rightV2.worldPosition, rightGradientY);
    FVector3 leftWorldNormal  = Interpolator::Vector3(leftV1.worldNormal,  leftV2.worldNormal,  leftGradientY);
    FVector3 rightWorldNormal = Interpolator::Vector3(rightV1.worldNormal, rightV2.worldNormal, rightGradientY);

    for (int x = leftX; x <= rightX; ++x) {
        // We set the gradient x to 100% by default in case the left 
        // and right x coordinates are the same to avoid division by zero
        // Or if the left and right x coordinates are the same
        // then we are only drawing one pixel in this row
        float gradientX = 1.f;
        if (leftX < rightX)
            gradientX = static_cast<float>(x - leftX) / static_cast<float>(rightX - leftX);

        // Horizontal interpolation between the left and right edges 
        // for the current x coordinate to determine the z coordinate
        float sampleZ = Interpolator::Scalar(leftZ, rightZ, gradientX);

        Color sampleColor{255, 255, 255, 255};
        if (gpu.Mode == GpuDrawMode::COLOR) {
            sampleColor = Interpolator::ColorLerp(leftColor, rightColor, gradientX);
        } else if (gpu.Mode == GpuDrawMode::TEXTURE && gpu.Texture != nullptr) {

            // With Texture mapping, we need to interpolate the UV coordinates across the triangle 
            // to determine which part of the texture to sample for the current pixel
            FVector2 sampleUV = Interpolator::Vector2(leftUV, rightUV, gradientX);
            const TextureCpu* texture = gpu.Texture;

            int textureX = static_cast<int>(static_cast<float>(texture->width  - 1) * sampleUV.x);
            int textureY = static_cast<int>(static_cast<float>(texture->height - 1) * (1.f - sampleUV.y));

            if (textureX < 0) textureX = 0; else if (textureX >= texture->width)  textureX = texture->width  - 1;
            if (textureY < 0) textureY = 0; else if (textureY >= texture->height) textureY = texture->height - 1;

            // The bidimensional texture coordinates (textureX, textureY) 
            // need to be converted to a one-dimensional index to access the pixel data in the texture's pixel array
            int textureIndex = (textureY * texture->width + textureX) * texture->pixelMemorySize;

            // Pixels in the texture are typically stored as a flat array of bytes
            //where each pixel is represented by a certain number of bytes (e.g., 4 bytes for RGBA)
            sampleColor.r = texture->pixels[textureIndex + 0];
            sampleColor.g = texture->pixels[textureIndex + 1];
            sampleColor.b = texture->pixels[textureIndex + 2];
            sampleColor.a = texture->pixels[textureIndex + 3];
        }

        FXColor sampleX{ sampleColor.r, sampleColor.g, sampleColor.b, sampleColor.a };

        // Ambient
        const float ambientIntensity = 0.1f;
        FXColor ambient = sampleX * ambientIntensity;

        // Per pixel position and direction of the point on the triangle in world space 
        // that corresponds to the current pixel we are drawing
        FVector3 worldPosition  = Interpolator::Vector3(leftWorldPosition,  rightWorldPosition,  gradientX);
        FVector3 worldNormal = Interpolator::Vector3(leftWorldNormal, rightWorldNormal, gradientX);

        // Diffuse (Lambert).
        FVector3 directionToLight = gpu.pointLightPosition - worldPosition;
        directionToLight.Normalize();
        worldNormal.Normalize();

        // This is the cosine of the angle between the direction to the light 
        // and the normal of the surface at that poin
        float cosLN = directionToLight.Dot(worldNormal);
        float lambert = std::clamp(cosLN, 0.f, 1.f);
        FXColor diffuse = sampleX * lambert;

        // Specular (Phong).
        FVector3 directionToEye = gpu.cameraPosition - worldPosition;
        directionToEye.Normalize();

        // This is the direction from the light to the point on the triangle
        // which is the opposite of the direction from the point to the light
        FVector3 directionLightToPoint = directionToLight * -1.f;
        // The vector representing the light bouncing off the surface of the triangle is determined 
        // by the angle between the direction to the light and the normal of the surface at that point
        // We can see it as twice the angle between the light vector multiplied by the normal vector minus the light vector
        // (R = 2(N dot L)N - L) or (R = L - 2(L dot N)N)
        FVector3 lightReflection = directionLightToPoint.Reflect(worldNormal);

        // This is the cosine of the angle between the direction to the eye and the direction of the reflected light
        float cosER = directionToEye.Dot(lightReflection);
        float specularValue = std::clamp(cosER, 0.f, 1.f);
        FXColor specularTint{255, 255, 255, 255};
        FXColor specular = specularTint * std::pow(specularValue, SHININESS_FACTOR);

        FXColor phong = ambient + diffuse + specular;

        screen->PutPixel(x, y, sampleZ, { phong.r, phong.g, phong.b, phong.a });
    }
}


void Rasterizer::DrawLine(int x1, int y1, int x2, int y2, Color color, Screen* screen) {
    int deltaX = std::abs(x1 - x2);
    int deltaY = std::abs(y1 - y2);
    // number of steps to take is the maximum of delta x and delta y to ensure we cover the entire line
    int steps = (deltaX < deltaY) ? deltaY : deltaX; 
    if (steps == 0) {
        screen->PutPixel(x1, y1, 0.f, color);
        return;
    }

    int stepX = (x1 < x2) ? 1 : -1; // step direction for x (1 if x1 < x2, -1 otherwise)
    int stepY = (y1 < y2) ? 1 : -1; // step direction for y (1 if y1 < y2, -1 otherwise)

    // number of pixels to step in x direction for each step
    float stepsX = static_cast<float>(deltaX) / static_cast<float>(steps) * static_cast<float>(stepX);
    // number of pixels to step in y direction for each step
    float stepsY = static_cast<float>(deltaY) / static_cast<float>(steps) * static_cast<float>(stepY);

    float x = static_cast<float>(x1); // starting x coordinate
    float y = static_cast<float>(y1); // starting y coordinate
    for (int i = 0; i <= steps; ++i) {
        // put pixel on the screen at the current x and y coordinates with the specified color
        screen->PutPixel(static_cast<int>(x), static_cast<int>(y), 0.f, color);
        x += stepsX; // increment x by the calculated step in x direction
        y += stepsY; // increment y by the calculated step in y direction
    }
}
void Rasterizer::DrawLine(IVector2 a, IVector2 b, Color color, Screen* screen) {
    DrawLine(a.x, a.y, b.x, b.y, color, screen);
}

void Rasterizer::DrawTriangle(IVector2 p1, IVector2 p2, IVector2 p3, Color color, Screen* screen) {
    // find the minimum x coordinate among the three vertices of the triangle
    int xMin = MathsUtils::Min3(p1.x, p2.x, p3.x); 
    // find the minimum y coordinate among the three vertices of the triangle
    int yMin = MathsUtils::Min3(p1.y, p2.y, p3.y);
    // find the maximum x coordinate among the three vertices of the triangle
    int xMax = MathsUtils::Max3(p1.x, p2.x, p3.x);
    // find the maximum y coordinate among the three vertices of the triangle
    int yMax = MathsUtils::Max3(p1.y, p2.y, p3.y);

    // iterate through each pixel in the bounding box defined by the minimum and maximum x and y coordinates
    for (int x = xMin; x <= xMax; ++x) {
        for (int y = yMin; y <= yMax; ++y) {
            IVector2 pt{ x, y };
            if (IsPointInTriangleBBox(pt, p1, p2, p3)) 
                screen->PutPixel(x, y, 0.f, color);
        }
    }
}

void Rasterizer::DrawCircle(IVector2 center, int radius, Color color, Screen* screen)
{
    const int radiusSquared = radius * radius;
    const int xMin = center.x - radius;
    const int xMax = center.x + radius;
    const int yMin = center.y - radius;
    const int yMax = center.y + radius;

    for (int x = xMin; x <= xMax; ++x) {
        for (int y = yMin; y <= yMax; ++y) {
            const int dx = x - center.x;
            const int dy = y - center.y;
            // TODO: Implement a function IsPointInCircle to avoid the square root calculation for better performance
            if ((dx * dx + dy * dy) <= radiusSquared)
                screen->PutPixel(x, y, 0.f, color);
        }
    }
}

void Rasterizer::DrawTriangle(const Gpu& gpu, const GpuVertex& v1, const GpuVertex& v2, const GpuVertex& v3, Screen* screen) {
    // Backface culling: skip triangle if it faces away from camera (unless two-sided rendering is enabled)
    if (!s_TwoSidedRenderingEnabled && s_EnableFaceCulling && !IsFrontFacing(gpu, v1, v2, v3))
        return;

    // reference_wrapper allows us to create an array of references to the vertices
    // which we can then sort based on their screen y coordinate
    std::array<std::reference_wrapper<const GpuVertex>, 3> points = { std::cref(v1), std::cref(v2), std::cref(v3) };
    // Sort the vertices by their screen y coordinate in ascending order
    std::sort(points.begin(), points.end(), [](const GpuVertex& p1, const GpuVertex& p2) {
        return p1.screenPosition.y < p2.screenPosition.y;
    });

    const GpuVertex& v1s = points[0].get();
    const GpuVertex& v2s = points[1].get();
    const GpuVertex& v3s = points[2].get();

    const IVector2& p1s = v1s.screenPosition;
    const IVector2& p2s = v2s.screenPosition;
    const IVector2& p3s = v3s.screenPosition;

    // Inverse slopes (dx/dy) to decide which side is left and which is right.
    // We take the vector that goes from p1 to p2 and p1 to p3
    // and calculate how much x changes for each change in y along those edges
    int dy12 = p2s.y - p1s.y;
    int dy13 = p3s.y - p1s.y;
    float invSlope12 = (dy12 != 0) ? static_cast<float>(p2s.x - p1s.x) / static_cast<float>(dy12) : 0.f;
    float invSlope13 = (dy13 != 0) ? static_cast<float>(p3s.x - p1s.x) / static_cast<float>(dy13) : 0.f;

    // <|  <- This is a triangle with a left edge that is less steep than the right edge (invSlope12 < invSlope13)
    // Depending on the relative slopes of the edges
    // we determine which edge is the left edge and which is the right edge of the triangle as we iterate down the y axis
    // if the slope of the edge from p1 to p2 is less than the slope of the edge from p1 to p3
    // then the edge from p1 to p2 is the left edge and the edge from p1 to p3 is the right edge
    if (invSlope12 < invSlope13) {
        for (int y = p1s.y; y <= p3s.y; ++y) {
            if (y < p2s.y) 
                RasterizeRow(gpu, y, v1s, v2s, v1s, v3s, screen);
            else
                RasterizeRow(gpu, y, v2s, v3s, v1s, v3s, screen);
        }
    } 
    // |>  <- This is a triangle with a left edge that is steeper than the right edge (invSlope12 >= invSlope13)
    else {
        for (int y = p1s.y; y <= p3s.y; ++y) {
            if (y < p2s.y)
                RasterizeRow(gpu, y, v1s, v3s, v1s, v2s, screen);
            else
                RasterizeRow(gpu, y, v1s, v3s, v2s, v3s, screen);
        }
    }
}

void Rasterizer::Draw(RasterMode mode, IVector2 a, IVector2 b, Color color, Screen* screen) {
    if (mode == RasterMode::DDA_LINE) 
        DrawLine(a, b, color, screen);
}
void Rasterizer::Draw(RasterMode mode, IVector2 p1, IVector2 p2, IVector2 p3, Color color, Screen* screen) {
    if (mode == RasterMode::BBOX_TRIANGLE)
        DrawTriangle(p1, p2, p3, color, screen);
    else if (mode == RasterMode::DDA_LINE) {
        // Wireframe convenience: three-edge outline.
        DrawLine(p1, p2, color, screen);
        DrawLine(p2, p3, color, screen);
        DrawLine(p3, p1, color, screen);
    }
}
void Rasterizer::Draw(RasterMode mode, const Gpu& gpu, const GpuVertex& v1, const GpuVertex& v2, const GpuVertex& v3, Screen* screen) {
    if (mode == RasterMode::SCANLINE_TRIANGLE) {
        DrawTriangle(gpu, v1, v2, v3, screen);
    } else if (mode == RasterMode::BBOX_TRIANGLE) {
        DrawTriangle(v1.screenPosition, v2.screenPosition, v3.screenPosition, v1.color, screen);
    } else if (mode == RasterMode::DDA_LINE) {
        DrawLine(v1.screenPosition, v2.screenPosition, v1.color, screen);
        DrawLine(v2.screenPosition, v3.screenPosition, v1.color, screen);
        DrawLine(v3.screenPosition, v1.screenPosition, v1.color, screen);
    }
}
