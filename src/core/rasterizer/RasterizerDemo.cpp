#include "core/rasterizer/RasterizerDemo.hpp"

#include "raylib.h"

#include "core/rasterizer/Rasterizer.hpp"
#include "core/rasterizer/Screen.hpp"
#include "utility/IVector2.hpp"
#include "utility/FVector3.hpp"

#include <array>

RasterizerDemo::RasterizerDemo(int width, int height)
    : _Width(width), _Height(height)
{ }

void RasterizerDemo::InitGame() {
    InitWindow(_Width, _Height, "Frostform Rasterizer Demo");
    SetTargetFPS(60);
}

namespace {

// Project a world-space point to screen space using a simple pinhole projection.
IVector2 Project(const FVector3& p, int screenW, int screenH) {
    // Camera looks down -Z. Move the point in front of the camera, then perspective-divide.
    const float fov = 1.5f; // larger -> narrower FOV (focal length factor)
    const float cameraOffset = 4.f; // pull the geometry forward of the camera plane

    float zPositionOfCam = p.z + cameraOffset;
    if (zPositionOfCam < 0.1f) zPositionOfCam = 0.1f;

    float halfW = static_cast<float>(screenW) * 0.5f;
    float halfH = static_cast<float>(screenH) * 0.5f;

    int sx = static_cast<int>(halfW + (p.x * fov / zPositionOfCam) * halfW);
    // Flip Y because screen Y grows downward.
    int sy = static_cast<int>(halfH - (p.y * fov / zPositionOfCam) * halfH);
    return { sx, sy };
}

} // namespace

void RasterizerDemo::Run() {
    Screen screen(_Width, _Height);

    // Triangle vertices in object space.
    std::array<FVector3, 3> baseVerts = {{
        {  0.0f,  0.9f, 0.0f },
        { -0.9f, -0.7f, 0.0f },
        {  0.9f, -0.7f, 0.0f }
    }};

    std::array<Color, 3> baseColors = {{ RED, GREEN, BLUE }};

    // 1 = DDA wireframe, 2 = bbox filled flat, 3 = scanline shaded.
    int mode = 3;

    float angle = 0.f;

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_ONE)) mode = 1;
        if (IsKeyPressed(KEY_TWO)) mode = 2;
        if (IsKeyPressed(KEY_THREE)) mode = 3;

        angle += 30.f * GetFrameTime();

        // Rotate verts in world space.
        std::array<FVector3, 3> world;
        for (int i = 0; i < 3; ++i) 
            world[i] = baseVerts[i].RotateY(angle);

        // Face normal (flat-shaded triangle).
        FVector3 edge1 = world[1] - world[0];
        FVector3 edge2 = world[2] - world[0];
        FVector3 normal = edge1.Cross(edge2);
        normal.Normalize();

        // Project to screen.
        std::array<IVector2, 3> screenPosition;
        std::array<float, 3> zPosition;
        for (int i = 0; i < 3; ++i) {
            screenPosition[i] = Project(world[i], _Width, _Height);
            // Use -z so that "closer" => greater value (matches Screen depth test).
            zPosition[i] = -world[i].z;
        }

        screen.Clear(BLACK);

        if (mode == 1) {
            Rasterizer::Draw(RasterMode::DDA_LINE, screenPosition[0], screenPosition[1], screenPosition[2], WHITE, &screen);
        } else if (mode == 2) {
            Rasterizer::Draw(RasterMode::BBOX_TRIANGLE, screenPosition[0], screenPosition[1], screenPosition[2], SKYBLUE, &screen);
        } else {
            Gpu gpu;
            gpu.Mode = GpuDrawMode::COLOR;
            gpu.pointLightPosition = { 0.f, 0.f, -2.f };
            gpu.cameraPosition = { 0.f, 0.f, -4.f };

            std::array<GpuVertex, 3> verts;
            for (int i = 0; i < 3; ++i) {
                verts[i].screenPosition = screenPosition[i];
                verts[i].color = baseColors[i];
                verts[i].zPosition = zPosition[i];
                verts[i].uv = { 0.f, 0.f };
                verts[i].worldPosition = world[i];
                verts[i].worldNormal = normal;
            }

            Rasterizer::Draw(RasterMode::SCANLINE_TRIANGLE, gpu, verts[0], verts[1], verts[2], &screen);
        }

        BeginDrawing();
        ClearBackground(BLACK);
        screen.Blit();

        DrawText("Rasterizer Demo", 10, 10, 20, WHITE);
        DrawText("[1] Wireframe (DDA)", 10, 40, 18, LIGHTGRAY);
        DrawText("[2] Filled (BBox)", 10, 62, 18, LIGHTGRAY);
        DrawText("[3] Scanline + Phong", 10, 84, 18, LIGHTGRAY);
        DrawText(TextFormat("Mode: %d", mode), 10, 110, 18, YELLOW);

        EndDrawing();
    }

    CloseWindow();
}
