#pragma once
#include "raylib.h"

// RAYLIB VECTOR EXTENDED
struct FVector2 : Vector2{
    FVector2(float x=0, float y=0);

    // Overload operators
    FVector2 operator+(const FVector2& other) const;
    FVector2 operator-(const FVector2& other) const;
    FVector2 operator*(const float other) const;
    FVector2 operator/(const float other) const;
    FVector2& operator*=(float other);
    // Utils
    float Dot(const FVector2& other) const;
    float Magnitude() const;
    static float SquaredLength (const FVector2& vector);
    static float Distance(const FVector2& A, const FVector2& B);
    FVector2 Normalized() const;
};
