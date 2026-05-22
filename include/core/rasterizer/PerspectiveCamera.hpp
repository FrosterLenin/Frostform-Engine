#pragma once

#include "core/rasterizer/ACamera.hpp"

// Perspective camera: objects farther away appear smaller (foreshortening)
class PerspectiveCamera : public ACamera {
public:
    PerspectiveCamera(int screenWidth, int screenHeight, float fov);

    IVector2 Project(FVector3 worldPoint) override;
    FVector3 WorldToCameraSpace(FVector3 worldPoint) override;
    FVector3 GetPosition() const override;
    void SetPosition(FVector3 position) override;
    bool IsFaceCulled(const FVector3& cp1, const FVector3& cp2, const FVector3& cp3) const override;

    float GetFov() const;
    void SetFov(float fov);

private:
    int _ScreenWidth;
    int _ScreenHeight;
    FVector3 _Position;
    float _Fov;
};
