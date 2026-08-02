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
    std::unique_ptr<TextureCpu> Texture = nullptr;
    GpuDrawMode Mode = GpuDrawMode::COLOR;
    FVector3 pointLightPosition {0.f, 0.f, 0.f};
    FVector3 cameraPosition {0.f, 0.f, 0.f};
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


class Rasterizer {
private:
    // higher values result in a smaller and sharper highlight
    static constexpr float SHININESS_FACTOR = 50.f; // Arbitrary shininess factor for specular highlights.
    static bool s_EnableFaceCulling;  // Toggle backface culling on/off
    static bool s_TwoSidedRenderingEnabled;  // Render both front and back faces
private:
    static bool IsPointInTriangleBBox(IVector2 p, IVector2 a, IVector2 b, IVector2 c);
    static bool IsFrontFacing(const Gpu& gpu, const GpuVertex& v1, const GpuVertex& v2, const GpuVertex& v3);
    static void RasterizeRow(const Gpu& gpu, int y,
                             const GpuVertex& leftV1, const GpuVertex& leftV2,
                             const GpuVertex& rightV1, const GpuVertex& rightV2,
                             Screen* screen);
public:
    static void SetFaceCullingEnabled(bool enabled);
    static bool IsFaceCullingEnabled();
    static void SetTwoSidedRenderingEnabled(bool enabled);
    static bool IsTwoSidedRenderingEnabled();

    // DDA (Digital Differential Analyzer algorithm) line rasterization with depth = 0
    static void DrawLine(int x1, int y1, int x2, int y2, Color color, Screen* screen);
    static void DrawLine(IVector2 pointA, IVector2 pointB, Color color, Screen* screen);

    // Bounding-box flat-color triangle, depth = 0.
    // In a Box we iterate over each pixel and check if it's in the triangle via edge functions.
    static void DrawTriangle(IVector2 pointA, IVector2 pointB, IVector2 pointC, Color color, Screen* screen);

    // Bounding-box circle fill, depth = 0.
    static void DrawCircle(IVector2 center, int radius, Color color, Screen* screen);

    // Scanline triangle with per-vertex attribute interpolation + Phong lighting.
    // We iterate over each horizontal row of the triangle 
    // and interpolate the left and right edges to find the start and end x coordinate for that row.
    static void DrawTriangle(const Gpu& gpu, const GpuVertex& v1, const GpuVertex& v2, const GpuVertex& v3, Screen* screen);

    // Mode-driven dispatch
    // Each overload asserts that `mode` matches its argument shape.
    static void Draw(RasterMode mode, IVector2 pointA, IVector2 pointB, Color color, Screen* screen);
    static void Draw(RasterMode mode, IVector2 pointA, IVector2 pointB, IVector2 pointC, Color color, Screen* screen);
    static void Draw(RasterMode mode, const Gpu& gpu, const GpuVertex& v1, const GpuVertex& v2, const GpuVertex& v3, Screen* screen);
};
