#include "utility/FVector2.hpp"
#include <cmath>
#include "raymath.h"

FVector2::FVector2(float x, float y) : Vector2{x,y}{};
FVector2 FVector2::operator+(const FVector2& other) const{
    return FVector2(x + other.x, y + other.y);
}
FVector2 FVector2::operator-(const FVector2& other) const{
    return FVector2(x - other.x, y - other.y);
}
FVector2 FVector2::operator*(const float other) const{
    return FVector2(x * other, y * other);
}
FVector2 FVector2::operator/(const float other) const{
    if (other == 0.0f) return {0,0};
    return FVector2(x / other, y / other);
}
FVector2& FVector2::operator*=(const float other) {
    *this = *this * other;
    return *this;
}
float FVector2::Dot(const FVector2& other) const{
    return x * other.x + y * other.y;
}
float FVector2::SquaredLength(const FVector2& vector){
    return vector.x * vector.x + vector.y * vector.y;
}
float FVector2::Distance(const FVector2& A, const FVector2& B){
    FVector2 C = A - B;
    return std::sqrt(SquaredLength(C));
}
FVector2 FVector2::Normalized() const{
    const float magnitude = std::sqrt(SquaredLength(*this));
    return *this/magnitude;
}
float FVector2::Magnitude() const{
    return Vector2Length({x, y});
}