#pragma once

#include "raylib.h"
#include <vector>

// Software framebuffer with a color buffer and depth buffer.
// The color buffer is uploaded to a raylib Texture2D each frame via Blit().
class Screen {
private:
    int _Width;
    int _Height;
    std::vector<Color> _ColorBuffer;
    std::vector<float> _DepthBuffer;
    Texture2D _Texture;
public:
    Screen(int width, int height);
    ~Screen();

    Screen(const Screen&) = delete;
    Screen& operator=(const Screen&) = delete;

    // Writes a pixel if z passes the depth test (greater-than is "closer" since
    // the rasterizer uses negated cartesian Z; see depth-test comment in PutPixel).
    void PutPixel(int x, int y, float z, Color color);

    void Clear(Color color);
    void Blit();

    int GetWidth() const { return _Width; }
    int GetHeight() const { return _Height; }


};
