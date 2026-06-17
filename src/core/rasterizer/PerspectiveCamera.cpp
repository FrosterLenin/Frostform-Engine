#include "core/rasterizer/PerspectiveCamera.hpp"

PerspectiveCamera::PerspectiveCamera(int screenWidth, int screenHeight, float fov)
    : _ScreenWidth(screenWidth), _ScreenHeight(screenHeight), _Position({0, 0, 0}), _Fov(fov)
{
}

IVector2 PerspectiveCamera::Project(FVector3 worldPoint) {
    FVector3 cameraPoint = WorldToCameraSpace(worldPoint);

    // Perspective divide: x and y scaled by fov, divided by depth (positive Z in camera space)
    float depth = cameraPoint.z;
    if (depth < 0.1f) depth = 0.1f;

    float halfW = _ScreenWidth * 0.5f;
    float halfH = _ScreenHeight * 0.5f;

    int sx = static_cast<int>(halfW + (cameraPoint.x * _Fov / depth) * halfW);
    int sy = static_cast<int>(halfH - (cameraPoint.y * _Fov / depth) * halfH);

    return {sx, sy};
}

IVector2 PerspectiveCamera::ProjectTopLeft2D(FVector2 worldPoint, float z) {
    const float halfWidth = _ScreenWidth * 0.5f;
    const float halfHeight = _ScreenHeight * 0.5f;

    const FVector3 centeredWorldPoint{
        worldPoint.x - halfWidth,
        halfHeight - worldPoint.y,
        z
    };

    return Project(centeredWorldPoint);
}

float PerspectiveCamera::GetProjectedDepth(FVector3 worldPoint) {
    return -WorldToCameraSpace(worldPoint).z;
}

Rectangle PerspectiveCamera::ProjectRectangleTopLeft(FVector2 position, FVector2 size, float z) {
    const IVector2 topLeft = ProjectTopLeft2D(position, z);
    const IVector2 bottomRight = ProjectTopLeft2D(position + size, z);

    Rectangle rectangle{};
    rectangle.x = static_cast<float>(topLeft.x);
    rectangle.y = static_cast<float>(topLeft.y);
    rectangle.width = static_cast<float>(bottomRight.x - topLeft.x);
    rectangle.height = static_cast<float>(bottomRight.y - topLeft.y);
    return rectangle;
}

FVector3 PerspectiveCamera::WorldToCameraSpace(FVector3 worldPoint) {
    return worldPoint - _Position;
}

FVector3 PerspectiveCamera::GetPosition() const {
    return _Position;
}

void PerspectiveCamera::SetPosition(FVector3 position) {
    _Position = position;
}

bool PerspectiveCamera::IsFaceCulled(const FVector3& cp1, const FVector3& cp2, const FVector3& cp3) const {
    FVector3 v12 = cp2 - cp1;
    FVector3 v13 = cp3 - cp1;

    FVector3 faceNormal = v12.Cross(v13);
    FVector3 cp1ToCamera = cp1 * -1.f;

    return cp1ToCamera.Dot(faceNormal) < 0;
}

float PerspectiveCamera::GetFov() const {
    return _Fov;
}

void PerspectiveCamera::SetFov(float fov) {
    _Fov = fov;
}
