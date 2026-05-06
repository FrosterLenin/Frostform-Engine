#pragma once
#include <cstdint>

// RGBA color used by the rasterizer's lighting math.
// Distinct from raylib's Color so that arithmetic operators (multiply by scalar,
// add) can be defined without polluting the raylib type.
struct FXColor {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;

    FXColor operator*(float scalar) const;
    FXColor operator+(const FXColor& other) const;
};
