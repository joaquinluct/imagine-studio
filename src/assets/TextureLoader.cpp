#include "TextureLoader.h"
#include "core/Log.h"

// STB Image - Single-header library for loading images
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <algorithm>

namespace Assets {

TextureData TextureLoader::LoadTexture(const std::string& filepath)
{
    TextureData data;
    
    if (filepath.empty())
    {
        CORE_LOG_ERROR("TextureLoader::LoadTexture: Empty filepath");
        return data;
    }
    
    // Check if file extension is supported
    if (!IsSupportedFormat(filepath))
    {
        CORE_LOG_ERROR("TextureLoader::LoadTexture: Unsupported format: " + filepath);
        return data;
    }
    
    // Load image with STB (force RGBA - 4 channels)
    // stbi_load returns unsigned char* with width * height * 4 bytes
    // Format: RGBA (Red, Green, Blue, Alpha) - 8 bits per channel
    int width = 0;
    int height = 0;
    int channels = 0;
    
    unsigned char* pixels = stbi_load(filepath.c_str(), &width, &height, &channels, STBI_rgb_alpha);
    
    if (!pixels)
    {
        CORE_LOG_ERROR("TextureLoader::LoadTexture: Failed to load: " + filepath);
        CORE_LOG_ERROR("  STB reason: " + std::string(stbi_failure_reason()));
        return data;
    }
    
    // Fill TextureData
    data.pixels = pixels;
    data.width = width;
    data.height = height;
    data.channels = channels;  // Original channels in file (3 or 4)
    
    CORE_LOG_INFO("TextureLoader: Loaded texture: " + filepath);
    CORE_LOG_INFO("  Size: " + std::to_string(width) + "x" + std::to_string(height));
    CORE_LOG_INFO("  Channels: " + std::to_string(channels) + " (converted to RGBA8)");
    
    return data;
}

void TextureLoader::FreeTextureData(TextureData& data)
{
    if (data.pixels)
    {
        stbi_image_free(data.pixels);
        data.pixels = nullptr;
        data.width = 0;
        data.height = 0;
        data.channels = 0;
    }
}

bool TextureLoader::IsSupportedFormat(const std::string& filepath)
{
    if (filepath.empty())
    {
        return false;
    }
    
    // Get file extension (lowercase)
    std::string ext = filepath.substr(filepath.find_last_of('.') + 1);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    
    // STB supports: JPG, PNG, TGA, BMP, PSD, GIF, HDR, PIC, PNM
    // We focus on common formats: jpg, jpeg, png, tga, bmp
    return ext == "jpg" || ext == "jpeg" || ext == "png" || 
           ext == "tga" || ext == "bmp";
}

} // namespace Assets
