#include "core/rasterizer/OrthographicCamera.hpp"

namespace {
constexpr float kMinOrthoSize = 0.0001f;
}

OrthographicCamera::OrthographicCamera(int screenWidth, int screenHeight, float orthoSize)
    : _ScreenWidth(screenWidth > 0 ? screenWidth : 1), _ScreenHeight(screenHeight > 0 ? screenHeight : 1), _Position({0, 0, 0}), _OrthoSize(orthoSize > kMinOrthoSize ? orthoSize : kMinOrthoSize)
{
    // Clamp constructor inputs so projection math never divides by zero.
    _AspectRatio = static_cast<float>(_ScreenWidth) / static_cast<float>(_ScreenHeight);
}

IVector2 OrthographicCamera::Project(FVector3 worldPoint) {
    FVector3 cameraPoint = WorldToCameraSpace(worldPoint);

    // Orthographic: ignore Z, scale x/y by orthoSize
    float planeX = cameraPoint.x / _OrthoSize;
    float planeY = cameraPoint.y / _OrthoSize;

    planeX /= _AspectRatio;

    // NDC {-1, 1} to Screen {w, h}
    int screenX = static_cast<int>((planeX + 1.f) * 0.5f * static_cast<float>(_ScreenWidth));
    int screenY = static_cast<int>((1.f - (planeY + 1.f) * 0.5f) * static_cast<float>(_ScreenHeight));

    return {screenX, screenY};
}

IVector2 OrthographicCamera::ProjectTopLeft2D(FVector2 worldPoint, float z) {
    const float halfWidth = _ScreenWidth * 0.5f;
    const float halfHeight = _ScreenHeight * 0.5f;

    const FVector3 centeredWorldPoint{
        worldPoint.x - halfWidth,
        halfHeight - worldPoint.y,
        z
    };

    return Project(centeredWorldPoint);
}

float OrthographicCamera::GetProjectedDepth(FVector3 worldPoint) {
    return -WorldToCameraSpace(worldPoint).z;
}

Rectangle OrthographicCamera::ProjectRectangleTopLeft(FVector2 position, FVector2 size, float z) {
    const IVector2 topLeft = ProjectTopLeft2D(position, z);
    const IVector2 bottomRight = ProjectTopLeft2D(position + size, z);

    Rectangle rectangle{};
    rectangle.x = static_cast<float>(topLeft.x);
    rectangle.y = static_cast<float>(topLeft.y);
    rectangle.width = static_cast<float>(bottomRight.x - topLeft.x);
    rectangle.height = static_cast<float>(bottomRight.y - topLeft.y);
    return rectangle;
}

FVector3 OrthographicCamera::WorldToCameraSpace(FVector3 worldPoint) {
    return worldPoint - _Position;
}

FVector3 OrthographicCamera::GetPosition() const {
    return _Position;
}

void OrthographicCamera::SetPosition(FVector3 position) {
    _Position = position;
}

bool OrthographicCamera::IsFaceCulled(const FVector3& cp1, const FVector3& cp2, const FVector3& cp3) const {
    FVector3 v12 = cp2 - cp1;
    FVector3 v13 = cp3 - cp1;

    FVector3 faceNormal = v12.Cross(v13);
    FVector3 cp1ToCamera = cp1 * -1.f;

    return cp1ToCamera.Dot(faceNormal) < 0;
}

float OrthographicCamera::GetOrthoSize() const {
    return _OrthoSize;
}

void OrthographicCamera::SetOrthoSize(float orthoSize) {
    // Keep ortho size strictly positive to avoid invalid projection scale
    _OrthoSize = orthoSize > kMinOrthoSize ? orthoSize : kMinOrthoSize;
}
