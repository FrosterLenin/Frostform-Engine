#pragma once

#include "utility/IVector2.hpp"

namespace FMaths {

    constexpr float PI2 = 3.1415926f;

    int Min3(int a, int b, int c);
    int Max3(int a, int b, int c);

    // Determinant of (v1->v2) and (v1->p). 
    // Used as an edge function for point-in-triangle tests during bounding-box rasterization.
    float Det(IVector2 p, IVector2 v1, IVector2 v2);
}
