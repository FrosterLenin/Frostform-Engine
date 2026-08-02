#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

// CPU-side texture used by the software rasterizer for UV sampling.
// Pixels are stored as a flat RGBA byte array.
class TextureCpu {
public:
    static std::unique_ptr<TextureCpu> LoadFromFile(const std::string& filePath);

    TextureCpu(int width, int height, int pixelMemorySize, std::vector<uint8_t> pixels);

    int width;
    int height;
    int pixelMemorySize; // total size of the single pixel in bytes (e.g., 4 for RGBA8)
    std::vector<uint8_t> pixels;
};
