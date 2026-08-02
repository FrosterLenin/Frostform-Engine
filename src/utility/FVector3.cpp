#include "utility/FVector3.hpp"
#include "utility/MathsUtils.hpp"

#include <cmath>

FVector3 FVector3::operator-(const FVector3& other) const {
    return {x - other.x, y - other.y, z - other.z};
}

FVector3 FVector3::operator+(const FVector3& other) const {
    return {x + other.x, y + other.y, z + other.z};
}

FVector3 FVector3::operator*(float scalar) const {
    return {x * scalar, y * scalar, z * scalar};
}

FVector3 FVector3::RotateY(float angleDegrees) const {
    float radians = angleDegrees * MathsUtils::fPI / 180.f;
    FVector3 result;
    result.x = std::cos(radians) * x - std::sin(radians) * z;
    result.y = y;
    result.z = std::sin(radians) * x + std::cos(radians) * z;
    return result;
}

void FVector3::Normalize() {
    float magnitude = Magnitude();
    if (magnitude == 0.f) return;
    x /= magnitude;
    y /= magnitude;
    z /= magnitude;
}

float FVector3::Magnitude() const {
    return std::sqrt(Dot(*this));
}

float FVector3::Dot(const FVector3& other) const {
    return x * other.x + y * other.y + z * other.z;
}

FVector3 FVector3::Reflect(const FVector3& norm) const {
    // R = I - 2 * dot(I, N) * N
    return *this - norm * (2.f * Dot(norm));
}

FVector3 FVector3::Cross(const FVector3& other) const {
    return {
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x
    };
}
