#pragma once

#include "raylib.h"
#include "utility/FVector2.hpp"
#include "utility/FVector3.hpp"

// Static helper class providing linear interpolation utilities 
// for scalars, vectors, and colors.
// for scalars, vectors, and colors.
// Used by the software rasterizer for per-pixel
// attribute interpolation across triangle edges and rows.
class Interpolator
{
public:
    Interpolator() = delete;

    static float Scalar(float a, float b, float gradient);
    static FVector2 Vector2(const FVector2& a, const FVector2& b, float gradient);
    static FVector3 Vector3(const FVector3& a, const FVector3& b, float gradient);
    static Color ColorLerp(const Color& a, const Color& b, float gradient);
};
