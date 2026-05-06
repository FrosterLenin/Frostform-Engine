#pragma once

#include "raylib.h"

#include "core/rasterizer/Screen.hpp"
#include "core/rasterizer/TextureCpu.hpp"
#include "core/enums/RasterMode.hpp"
#include "core/enums/GpuDrawMode.hpp"
#include "utility/FVector2.hpp"
#include "utility/IVector2.hpp"
#include "utility/FVector3.hpp"

// Per-draw rasterizer state for the scanline path (lighting, shading mode, texture).
struct Gpu {
    TextureCpu* Texture = nullptr;
    GpuDrawMode Mode = GpuDrawMode::COLOR;
    FVector3 pointLightPosition{0.f, 0.f, 0.f};
    FVector3 cameraPosition{0.f, 0.f, 0.f};
};

// Per-vertex inputs for the scanline triangle path.
struct GpuVertex {
    IVector2 screenPosition;
    Color color;
    float zPosition;
    FVector2 uv;
    FVector3 worldPosition;
    FVector3 worldNormal;
};

// Unified software rasterizer. Provides:
//   * explicit named methods (DrawLine / DrawTriangle) for direct calls
//   * a Draw(RasterMode, ...) overload set for mode-driven dispatch
class Rasterizer {
public:
    // Direct API 

    // DDA line, depth = 0.
    static void DrawLine(int x1, int y1, int x2, int y2, Color color, Screen* screen);
    static void DrawLine(IVector2 pointA, IVector2 pointB, Color color, Screen* screen);

    // Bounding-box flat-color triangle, depth = 0.
    static void DrawTriangle(IVector2 pointA, IVector2 pointB, IVector2 pointC, Color color, Screen* screen);

    // Scanline triangle with per-vertex attribute interpolation + Phong lighting.
    static void DrawTriangle(const Gpu& gpu, const GpuVertex& v1, const GpuVertex& v2, const GpuVertex& v3, Screen* screen);

    // Mode-driven dispatch
    // Each overload asserts that `mode` matches its argument shape.

    static void Draw(RasterMode mode, IVector2 pointA, IVector2 pointB, Color color, Screen* screen);
    static void Draw(RasterMode mode, IVector2 pointA, IVector2 pointB, IVector2 pointC, Color color, Screen* screen);
    static void Draw(RasterMode mode, const Gpu& gpu, const GpuVertex& v1, const GpuVertex& v2, const GpuVertex& v3, Screen* screen);
};
