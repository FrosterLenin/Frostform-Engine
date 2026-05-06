#include "utility/FXColor.hpp"

#include <algorithm>
#include <cstdint>

namespace {
    inline uint8_t SaturateToU8(float v) {
        if (v < 0.f) return 0;
        if (v > 255.f) return 255;
        return static_cast<uint8_t>(v);
    }
    inline uint8_t SaturateAddU8(int a, int b) {
        int sum = a + b;
        if (sum < 0) return 0;
        if (sum > 255) return 255;
        return static_cast<uint8_t>(sum);
    }
}

FXColor FXColor::operator*(float scalar) const {
    return {
        SaturateToU8(static_cast<float>(r) * scalar),
        SaturateToU8(static_cast<float>(g) * scalar),
        SaturateToU8(static_cast<float>(b) * scalar),
        SaturateToU8(static_cast<float>(a) * scalar)
    };
}

FXColor FXColor::operator+(const FXColor& other) const {
    return {
        SaturateAddU8(r, other.r),
        SaturateAddU8(g, other.g),
        SaturateAddU8(b, other.b),
        SaturateAddU8(a, other.a)
    };
}
