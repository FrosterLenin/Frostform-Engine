#pragma once

// Standalone runner for the software rasterizer.
// Mirrors the launcher pattern of PongGame / SpaceInvaders: owns its own raylib
// window and main loop. Renders a rotating triangle in three modes selectable
// at runtime (1 = wireframe, 2 = filled bbox, 3 = scanline + Phong).
class RasterizerDemo {
private:
    int _Width;
    int _Height;
public:
    RasterizerDemo(int width = 800, int height = 450);

    void InitGame();
    void Run();
};
