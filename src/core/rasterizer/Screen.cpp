#include "core/rasterizer/Screen.hpp"

#include <algorithm>
#include <limits>

Screen::Screen(int width, int height)
    : _Width(width), _Height(height)
{
    _ColorBuffer.resize(static_cast<size_t>(width) * static_cast<size_t>(height));
    _DepthBuffer.resize(static_cast<size_t>(width) * static_cast<size_t>(height));

    Image img = {
        _ColorBuffer.data(),
        _Width,
        _Height,
        1,
        PIXELFORMAT_UNCOMPRESSED_R8G8B8A8
    };

    _Texture = LoadTextureFromImage(img);
}

Screen::~Screen() {
    UnloadTexture(_Texture);
}

void Screen::PutPixel(int x, int y, float z, Color color) {
    if (x < 0 || x >= _Width)  return;
    if (y < 0 || y >= _Height) return;

    int index = y * _Width + x;

    // Depth test: rasterizer feeds Z that grows as the pixel gets closer to the
    // camera (cartesian Y/Z are inverted in screen space), so a *greater* Z wins.
    float prev_z = _DepthBuffer[index];
    if (prev_z > z) return;

    _DepthBuffer[index] = z;
    _ColorBuffer[index] = color;
}

void Screen::Clear(Color color) {
    std::fill(_ColorBuffer.begin(), _ColorBuffer.end(), color);
    std::fill(_DepthBuffer.begin(), _DepthBuffer.end(), std::numeric_limits<float>::lowest());
}

void Screen::Blit() {
    UpdateTexture(_Texture, _ColorBuffer.data());
    DrawTexture(_Texture, 0, 0, WHITE);
}
