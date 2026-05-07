#include "utility/MathsUtils.hpp"

#include <algorithm>

namespace MathsUtils {

    int Min3(int a, int b, int c) {
        return std::min(std::min(a, b), c);
    }

    int Max3(int a, int b, int c) {
        return std::max(std::max(a, b), c);
    }

    float Det(IVector2 p, IVector2 v1, IVector2 v2) {
        int vx = v2.x - v1.x;
        int vy = v2.y - v1.y;
        int px = p.x - v1.x;
        int py = p.y - v1.y;
        return static_cast<float>(vx * py - vy * px);
    }
}
