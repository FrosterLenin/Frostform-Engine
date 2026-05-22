#include "core/rasterizer/OrthographicCamera.hpp"

OrthographicCamera::OrthographicCamera(int screenWidth, int screenHeight, float orthoSize)
    : _ScreenWidth(screenWidth), _ScreenHeight(screenHeight), _Position({0, 0, 0}), _OrthoSize(orthoSize)
{
    _AspectRatio = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);
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
    _OrthoSize = orthoSize;
}
