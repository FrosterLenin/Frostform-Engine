#include "core/rasterizer/Rasterizer.hpp"

#include "utility/FMaths.hpp"
#include "utility/FXColor.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <functional>


// ------------------------------------------------------------------------
// Interpolation helpers (linear).
// ------------------------------------------------------------------------

float InterpolateScalar(float a, float b, float gradient) {
    return a + gradient * (b - a);
}

FVector2 InterpolateVector2f(const FVector2& a, const FVector2& b, float gradient) {
    return { InterpolateScalar(a.x, b.x, gradient), InterpolateScalar(a.y, b.y, gradient) };
}

FVector3 InterpolateVector3f(const FVector3& a, const FVector3& b, float gradient) {
    return {
        InterpolateScalar(a.x, b.x, gradient),
        InterpolateScalar(a.y, b.y, gradient),
        InterpolateScalar(a.z, b.z, gradient)
    };
}

Color InterpolateColor(const Color& a, const Color& b, float gradient) {
    Color result;
    result.r = static_cast<uint8_t>(InterpolateScalar(a.r, b.r, gradient));
    result.g = static_cast<uint8_t>(InterpolateScalar(a.g, b.g, gradient));
    result.b = static_cast<uint8_t>(InterpolateScalar(a.b, b.b, gradient));
    result.a = static_cast<uint8_t>(InterpolateScalar(a.a, b.a, gradient));
    return result;
}

// ------------------------------------------------------------------------
// Bounding-box triangle helpers.
// ------------------------------------------------------------------------

bool IsPointInTriangleBBox(IVector2 p, IVector2 a, IVector2 b, IVector2 c) {
    bool b1 = FMaths::Det(p, a, b) < 0;
    bool b2 = FMaths::Det(p, b, c) < 0;
    bool b3 = FMaths::Det(p, c, a) < 0;
    return (b1 == b2) && (b2 == b3);
}

// ------------------------------------------------------------------------
// Scanline row.
// ------------------------------------------------------------------------

void RasterizeRow(const Gpu& gpu, int y,
                  const GpuVertex& leftV1, const GpuVertex& leftV2,
                  const GpuVertex& rightV1, const GpuVertex& rightV2,
                  Screen* screen)
{
    const IVector2& leftSP1  = leftV1.screenPosition;
    const IVector2& leftSP2  = leftV2.screenPosition;
    const IVector2& rightSP1 = rightV1.screenPosition;
    const IVector2& rightSP2 = rightV2.screenPosition;

    // Distance of the current y from the top of the edge, normalized to [0,1].
    float leftGradY = 1.f;
    if (leftSP2.y != leftSP1.y) {
        leftGradY = static_cast<float>(y - leftSP1.y) / static_cast<float>(leftSP2.y - leftSP1.y);
    }
    float rightGradY = 1.f;
    if (rightSP2.y != rightSP1.y) {
        rightGradY = static_cast<float>(y - rightSP1.y) / static_cast<float>(rightSP2.y - rightSP1.y);
    }

    int leftX  = static_cast<int>(InterpolateScalar(static_cast<float>(leftSP1.x),  static_cast<float>(leftSP2.x),  leftGradY));
    int rightX = static_cast<int>(InterpolateScalar(static_cast<float>(rightSP1.x), static_cast<float>(rightSP2.x), rightGradY));

    float leftZ  = InterpolateScalar(leftV1.zPosition,  leftV2.zPosition,  leftGradY);
    float rightZ = InterpolateScalar(rightV1.zPosition, rightV2.zPosition, rightGradY);

    Color     leftColor{}, rightColor{};
    FVector2 leftUV{}, rightUV{};

    if (gpu.Mode == GpuDrawMode::COLOR) {
        leftColor  = InterpolateColor(leftV1.color,  leftV2.color,  leftGradY);
        rightColor = InterpolateColor(rightV1.color, rightV2.color, rightGradY);
    } else if (gpu.Mode == GpuDrawMode::TEXTURE) {
        leftUV  = InterpolateVector2f(leftV1.uv,  leftV2.uv,  leftGradY);
        rightUV = InterpolateVector2f(rightV1.uv, rightV2.uv, rightGradY);
    }

    FVector3 leftWorldPos  = InterpolateVector3f(leftV1.worldPosition,  leftV2.worldPosition,  leftGradY);
    FVector3 rightWorldPos = InterpolateVector3f(rightV1.worldPosition, rightV2.worldPosition, rightGradY);
    FVector3 leftWorldNorm  = InterpolateVector3f(leftV1.worldNormal,  leftV2.worldNormal,  leftGradY);
    FVector3 rightWorldNorm = InterpolateVector3f(rightV1.worldNormal, rightV2.worldNormal, rightGradY);

    for (int x = leftX; x <= rightX; ++x) {
        float gradX = 1.f;
        if (leftX < rightX) {
            gradX = static_cast<float>(x - leftX) / static_cast<float>(rightX - leftX);
        }

        float sampleZ = InterpolateScalar(leftZ, rightZ, gradX);

        Color sampleColor{255, 255, 255, 255};
        if (gpu.Mode == GpuDrawMode::COLOR) {
            sampleColor = InterpolateColor(leftColor, rightColor, gradX);
        } else if (gpu.Mode == GpuDrawMode::TEXTURE && gpu.Texture != nullptr) {
            FVector2 sampleUV = InterpolateVector2f(leftUV, rightUV, gradX);
            const TextureCpu* tex = gpu.Texture;

            int tx = static_cast<int>(static_cast<float>(tex->width  - 1) * sampleUV.x);
            int ty = static_cast<int>(static_cast<float>(tex->height - 1) * (1.f - sampleUV.y));

            if (tx < 0) tx = 0; else if (tx >= tex->width)  tx = tex->width  - 1;
            if (ty < 0) ty = 0; else if (ty >= tex->height) ty = tex->height - 1;

            int idx = (ty * tex->width + tx) * tex->pixelMemorySize;
            sampleColor.r = tex->pixels[idx + 0];
            sampleColor.g = tex->pixels[idx + 1];
            sampleColor.b = tex->pixels[idx + 2];
            sampleColor.a = tex->pixels[idx + 3];
        }

        FXColor sampleX{ sampleColor.r, sampleColor.g, sampleColor.b, sampleColor.a };

        // Ambient term.
        const float ambientIntensity = 0.1f;
        FXColor ambient = sampleX * ambientIntensity;

        // Per-pixel world-space position and normal.
        FVector3 worldPos  = InterpolateVector3f(leftWorldPos,  rightWorldPos,  gradX);
        FVector3 worldNorm = InterpolateVector3f(leftWorldNorm, rightWorldNorm, gradX);

        // Diffuse (Lambert).
        FVector3 dirToLight = gpu.pointLightPosition - worldPos;
        dirToLight.Normalize();
        worldNorm.Normalize();

        float cosLN   = dirToLight.Dot(worldNorm);
        float lambert = std::clamp(cosLN, 0.f, 1.f);
        FXColor diffuse = sampleX * lambert;

        // Specular (Phong).
        FVector3 dirToEye = gpu.cameraPosition - worldPos;
        dirToEye.Normalize();

        FVector3 dirLightToPoint = dirToLight * -1.f;
        FVector3 lightRefl = dirLightToPoint.Reflect(worldNorm);

        float cosER = dirToEye.Dot(lightRefl);
        float specularValue = std::clamp(cosER, 0.f, 1.f);
        FXColor specularTint{255, 255, 255, 255};
        FXColor specular = specularTint * std::pow(specularValue, 50.f);

        FXColor phong = ambient + diffuse + specular;

        screen->PutPixel(x, y, sampleZ, { phong.r, phong.g, phong.b, phong.a });
    }
}


// ------------------------------------------------------------------------
// DDA line.
// ------------------------------------------------------------------------

void Rasterizer::DrawLine(int x1, int y1, int x2, int y2, Color color, Screen* screen) {
    int dx = std::abs(x1 - x2);
    int dy = std::abs(y1 - y2);
    int steps = (dx < dy) ? dy : dx;
    if (steps == 0) {
        screen->PutPixel(x1, y1, 0.f, color);
        return;
    }

    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;

    float stepsX = static_cast<float>(dx) / static_cast<float>(steps) * static_cast<float>(sx);
    float stepsY = static_cast<float>(dy) / static_cast<float>(steps) * static_cast<float>(sy);

    float x = static_cast<float>(x1);
    float y = static_cast<float>(y1);
    for (int i = 0; i <= steps; ++i) {
        screen->PutPixel(static_cast<int>(x), static_cast<int>(y), 0.f, color);
        x += stepsX;
        y += stepsY;
    }
}

void Rasterizer::DrawLine(IVector2 a, IVector2 b, Color color, Screen* screen) {
    DrawLine(a.x, a.y, b.x, b.y, color, screen);
}

// ------------------------------------------------------------------------
// Bounding-box flat triangle.
// ------------------------------------------------------------------------

void Rasterizer::DrawTriangle(IVector2 p1, IVector2 p2, IVector2 p3, Color color, Screen* screen) {
    int xMin = FMaths::Min3(p1.x, p2.x, p3.x);
    int yMin = FMaths::Min3(p1.y, p2.y, p3.y);
    int xMax = FMaths::Max3(p1.x, p2.x, p3.x);
    int yMax = FMaths::Max3(p1.y, p2.y, p3.y);

    for (int x = xMin; x <= xMax; ++x) {
        for (int y = yMin; y <= yMax; ++y) {
            IVector2 pt{ x, y };
            if (IsPointInTriangleBBox(pt, p1, p2, p3)) {
                screen->PutPixel(x, y, 0.f, color);
            }
        }
    }
}

// ------------------------------------------------------------------------
// Scanline triangle with shading.
// ------------------------------------------------------------------------

void Rasterizer::DrawTriangle(const Gpu& gpu, const GpuVertex& v1, const GpuVertex& v2, const GpuVertex& v3, Screen* screen) {
    std::array<std::reference_wrapper<const GpuVertex>, 3> points = { std::cref(v1), std::cref(v2), std::cref(v3) };
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
    int dy12 = p2s.y - p1s.y;
    int dy13 = p3s.y - p1s.y;
    float invSlope12 = (dy12 != 0) ? static_cast<float>(p2s.x - p1s.x) / static_cast<float>(dy12) : 0.f;
    float invSlope13 = (dy13 != 0) ? static_cast<float>(p3s.x - p1s.x) / static_cast<float>(dy13) : 0.f;

    if (invSlope12 < invSlope13) {
        for (int y = p1s.y; y <= p3s.y; ++y) {
            if (y < p2s.y) {
                RasterizeRow(gpu, y, v1s, v2s, v1s, v3s, screen);
            } else {
                RasterizeRow(gpu, y, v2s, v3s, v1s, v3s, screen);
            }
        }
    } else {
        for (int y = p1s.y; y <= p3s.y; ++y) {
            if (y < p2s.y) {
                RasterizeRow(gpu, y, v1s, v3s, v1s, v2s, screen);
            } else {
                RasterizeRow(gpu, y, v1s, v3s, v2s, v3s, screen);
            }
        }
    }
}

// ------------------------------------------------------------------------
// Mode-driven dispatch.
// ------------------------------------------------------------------------

void Rasterizer::Draw(RasterMode mode, IVector2 a, IVector2 b, Color color, Screen* screen) {
    if (mode == RasterMode::DDA_LINE) {
        DrawLine(a, b, color, screen);
    }
}

void Rasterizer::Draw(RasterMode mode, IVector2 p1, IVector2 p2, IVector2 p3, Color color, Screen* screen) {
    if (mode == RasterMode::BBOX_TRIANGLE) {
        DrawTriangle(p1, p2, p3, color, screen);
    } else if (mode == RasterMode::DDA_LINE) {
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
