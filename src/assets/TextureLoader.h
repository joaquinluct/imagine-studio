#pragma once

#include <string>

namespace Assets {

// v2.1.0 H1.1 - TextureLoader for loading JPG/PNG textures with STB
// Loads texture data from disk into CPU memory (unsigned char* pixels)
// Use with DX12ResourceManager to upload to GPU
struct TextureData {
    unsigned char* pixels = nullptr;  // RGBA8 pixel data (must be freed with FreeTextureData)
    int width = 0;
    int height = 0;
    int channels = 0;  // Original channels in file (3=RGB, 4=RGBA)
    
    bool IsValid() const { return pixels != nullptr && width > 0 && height > 0; }
};

class TextureLoader {
public:
    // Load texture from disk (JPG/PNG/TGA/BMP)
    // Returns TextureData with RGBA8 pixels (always 4 channels, even if source is RGB)
    // Caller MUST call FreeTextureData() when done
    static TextureData LoadTexture(const std::string& filepath);
    
    // Free texture data allocated by LoadTexture
    static void FreeTextureData(TextureData& data);
    
    // Check if file extension is supported
    static bool IsSupportedFormat(const std::string& filepath);
};

} // namespace Assets
