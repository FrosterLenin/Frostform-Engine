#pragma once

#include "core/rasterizer/ACamera.hpp"

// Orthographic camera: objects are the same size regardless of distance, no foreshortening
class OrthographicCamera : public ACamera {
public:
    OrthographicCamera(int screenWidth, int screenHeight, float orthoSize);

    IVector2 Project(FVector3 worldPoint) override;
    IVector2 ProjectTopLeft2D(FVector2 worldPoint, float z = 0.0f) override;
    float GetProjectedDepth(FVector3 worldPoint) override;
    Rectangle ProjectRectangleTopLeft(FVector2 position, FVector2 size, float z = 0.0f) override;
    FVector3 WorldToCameraSpace(FVector3 worldPoint) override;
    FVector3 GetPosition() const override;
    void SetPosition(FVector3 position) override;
    bool IsFaceCulled(const FVector3& cp1, const FVector3& cp2, const FVector3& cp3) const override;

    float GetOrthoSize() const;
    void SetOrthoSize(float orthoSize);

private:
    int _ScreenWidth;
    int _ScreenHeight;
    FVector3 _Position;
    float _OrthoSize;
    float _AspectRatio;
};
