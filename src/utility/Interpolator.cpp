#include "utility/Interpolator.hpp"

#include <cstdint>

float Interpolator::Scalar(float a, float b, float gradient)
{
    // From a to b: a is the start, b is the end, gradient is the [0,1]
    // position between them. Multiplying the (b-a) delta by the gradient
    // yields how much to add to a to land on the interpolated value.
    return a + gradient * (b - a);
}

FVector2 Interpolator::Vector2(const FVector2& a, const FVector2& b, float gradient)
{
    return {
        Scalar(a.x, b.x, gradient),
        Scalar(a.y, b.y, gradient)
    };
}

FVector3 Interpolator::Vector3(const FVector3& a, const FVector3& b, float gradient)
{
    return {
        Scalar(a.x, b.x, gradient),
        Scalar(a.y, b.y, gradient),
        Scalar(a.z, b.z, gradient)
    };
}

Color Interpolator::ColorLerp(const Color& a, const Color& b, float gradient)
{
    // Colors are typically represented as 8-bit unsigned integers (0-255) for each channel (red, green, blue, alpha)
    // So we need to interpolate each channel separately and then cast the result back to uint8_t
    Color result;
    result.r = static_cast<uint8_t>(Scalar(a.r, b.r, gradient));
    result.g = static_cast<uint8_t>(Scalar(a.g, b.g, gradient));
    result.b = static_cast<uint8_t>(Scalar(a.b, b.b, gradient));
    result.a = static_cast<uint8_t>(Scalar(a.a, b.a, gradient));
    return result;
}
