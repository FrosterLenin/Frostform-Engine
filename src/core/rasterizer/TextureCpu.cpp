#include "core/rasterizer/TextureCpu.hpp"
#include "raylib.h"
#include <iostream>

std::unique_ptr<TextureCpu> TextureCpu::LoadFromFile(const std::string& filePath) {
    Image image = LoadImage(filePath.c_str());
    if (!IsImageValid(image)) {
        std::cout << "Error loading image: " << filePath << "\n";
        return nullptr;
    }

    // Force RGBA8 layout so the rasterizer can index pixels predictably.
    ImageFormat(&image, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);


    int bytesPerPixel = 4; // RGBA8 has 4 bytes per pixel (R, G, B, A)
    size_t textureSize = static_cast<size_t>(image.width) * static_cast<size_t>(image.height) * static_cast<size_t>(bytesPerPixel);
    uint8_t* data = reinterpret_cast<uint8_t*>(image.data);

    std::vector<uint8_t> pixels(data, data + textureSize);

    std::unique_ptr<TextureCpu> texture = std::make_unique<TextureCpu>(
        image.width,
        image.height,
        bytesPerPixel,
        std::move(pixels)
    );
    UnloadImage(image);
    return texture;
}

TextureCpu::TextureCpu(int w, int h, int pixelMemorySize, std::vector<uint8_t> pxs)
    : width(w), height(h), pixelMemorySize(pixelMemorySize), pixels(std::move(pxs))
{ }
