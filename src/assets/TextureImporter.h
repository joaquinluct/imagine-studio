#pragma once

#include <cstdint>
#include <string>

namespace Assets {

// Texture pixel data structure
struct TextureData {
    unsigned char* pixels;  // Pixel data (RGBA format if channels == 4)
    int width;              // Image width in pixels
    int height;             // Image height in pixels
    int channels;           // Number of channels (1=Grey, 2=Grey+Alpha, 3=RGB, 4=RGBA)
    std::string path;       // Original file path
    
    TextureData()
        : pixels(nullptr)
        , width(0)
        , height(0)
        , channels(0)
        , path("")
    {}
    
    // Check if texture data is valid
    bool IsValid() const {
        return pixels != nullptr && width > 0 && height > 0 && channels > 0;
    }
};

// Texture importer using stb_image
class TextureImporter {
public:
    // Import texture from file
    // Returns TextureData with pixel data (caller must call FreeTextureData)
    // Throws std::runtime_error on failure
    static TextureData ImportTexture(const std::string& path, int desiredChannels = 4);
    
    // Import texture with automatic RGBA conversion
    static TextureData ImportTextureRGBA(const std::string& path);
    
    // Free texture pixel data
    static void FreeTextureData(TextureData& data);
    
    // Get texture info without loading pixels
    static bool GetTextureInfo(const std::string& path, int& width, int& height, int& channels);
    
    // Check if file format is supported
    static bool IsSupportedFormat(const std::string& path);
    
    // Get last error message
    static const char* GetLastError();
    
private:
    TextureImporter() = delete;  // Static class, no instances
};

} // namespace Assets
