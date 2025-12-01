#pragma once

#include <string>
#include <cstring>  // For memcpy

// Forward declare stbi_image_free
extern "C" {
    void stbi_image_free(void* retval_from_stbi_load);
}

namespace Assets {

// v2.1.0 H1.1 - TextureLoader for loading JPG/PNG textures with STB
// Loads texture data from disk into CPU memory (unsigned char* pixels)
// Use with DX12ResourceManager to upload to GPU
// RAII: Rule of Five implemented to prevent double-free
struct TextureData {
    unsigned char* pixels = nullptr;  // RGBA8 pixel data (owned, freed in destructor)
    int width = 0;
    int height = 0;
    int channels = 0;  // Original channels in file (3=RGB, 4=RGBA)
    
    // Constructor
    TextureData() = default;
    
    // Destructor - Free owned memory
    inline ~TextureData()
    {
        if (pixels)
        {
            stbi_image_free(pixels);
            pixels = nullptr;
        }
    }
    
    // Copy constructor - Deep copy
    inline TextureData(const TextureData& other)
        : width(other.width), height(other.height), channels(other.channels)
    {
        if (other.pixels && other.width > 0 && other.height > 0)
        {
            // Deep copy - allocate new memory and copy pixel data
            size_t size = static_cast<size_t>(other.width) * other.height * 4; // RGBA8
            pixels = static_cast<unsigned char*>(malloc(size));
            if (pixels)
            {
                std::memcpy(pixels, other.pixels, size);
            }
            else
            {
                width = 0;
                height = 0;
                channels = 0;
            }
        }
        else
        {
            pixels = nullptr;
        }
    }
    
    // Copy assignment - Deep copy
    inline TextureData& operator=(const TextureData& other)
    {
        if (this != &other)
        {
            // Free existing memory
            if (pixels)
            {
                stbi_image_free(pixels);
                pixels = nullptr;
            }
            
            // Copy metadata
            width = other.width;
            height = other.height;
            channels = other.channels;
            
            // Deep copy pixel data
            if (other.pixels && other.width > 0 && other.height > 0)
            {
                size_t size = static_cast<size_t>(other.width) * other.height * 4; // RGBA8
                pixels = static_cast<unsigned char*>(malloc(size));
                if (pixels)
                {
                    std::memcpy(pixels, other.pixels, size);
                }
                else
                {
                    width = 0;
                    height = 0;
                    channels = 0;
                }
            }
        }
        return *this;
    }
    
    // Move constructor - Transfer ownership
    inline TextureData(TextureData&& other) noexcept
        : pixels(other.pixels), width(other.width), height(other.height), channels(other.channels)
    {
        // Transfer ownership - leave other in valid but empty state
        other.pixels = nullptr;
        other.width = 0;
        other.height = 0;
        other.channels = 0;
    }
    
    // Move assignment - Transfer ownership
    inline TextureData& operator=(TextureData&& other) noexcept
    {
        if (this != &other)
        {
            // Free existing memory
            if (pixels)
            {
                stbi_image_free(pixels);
            }
            
            // Transfer ownership
            pixels = other.pixels;
            width = other.width;
            height = other.height;
            channels = other.channels;
            
            // Leave other in valid but empty state
            other.pixels = nullptr;
            other.width = 0;
            other.height = 0;
            other.channels = 0;
        }
        return *this;
    }
    
    bool IsValid() const { return pixels != nullptr && width > 0 && height > 0; }
};

class TextureLoader {
public:
    // Load texture from disk (JPG/PNG/TGA/BMP)
    // Returns TextureData with RGBA8 pixels (always 4 channels, even if source is RGB)
    // TextureData owns the memory and will free it automatically on destruction
    static TextureData LoadTexture(const std::string& filepath);
    
    // Free texture data (deprecated - now handled by TextureData destructor)
    // Kept for backward compatibility
    static void FreeTextureData(TextureData& data);
    
    // Check if file extension is supported
    static bool IsSupportedFormat(const std::string& filepath);
};

} // namespace Assets
