#pragma once
#include "raylib.h"
// 3D float vector used by the software rasterizer for world-space positions,
// normals, and lighting math.
struct FVector3 : public Vector3 {

    // Overload operators
    FVector3 operator-(const FVector3& other) const;
    FVector3 operator+(const FVector3& other) const;
    FVector3 operator*(float scalar) const;

    // Utils
    FVector3 RotateY(float angleDegrees) const;
    void Normalize();
    float Magnitude() const;
    float Dot(const FVector3& other) const;
    FVector3 Reflect(const FVector3& norm) const;
    FVector3 Cross(const FVector3& other) const;
};
