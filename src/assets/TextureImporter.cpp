#include "TextureImporter.h"
#include <algorithm>
#include <stdexcept>

// Include stb_image header (implementation is in external/stb/stb_image_impl.cpp)
#include "../../external/stb/stb_image.h"

namespace Assets {

// Helper function to check if string ends with suffix (C++14 compatible)
static bool EndsWith(const std::string& str, const std::string& suffix) {
    if (suffix.size() > str.size()) return false;
    return str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

TextureData TextureImporter::ImportTexture(const std::string& path, int desiredChannels)
{
    TextureData data;
    data.path = path;
    
    // Load image using stb_image
    data.pixels = stbi_load(path.c_str(), &data.width, &data.height, &data.channels, desiredChannels);
    
    if (!data.pixels) {
        const char* error = stbi_failure_reason();
        throw std::runtime_error(std::string("Failed to load texture: ") + path + " - " + error);
    }
    
    // If desiredChannels was specified, update channels
    if (desiredChannels > 0) {
        data.channels = desiredChannels;
    }
    
    return data;
}

TextureData TextureImporter::ImportTextureRGBA(const std::string& path)
{
    // Force 4 channels (RGBA)
    return ImportTexture(path, 4);
}

void TextureImporter::FreeTextureData(TextureData& data)
{
    if (data.pixels) {
        stbi_image_free(data.pixels);
        data.pixels = nullptr;
        data.width = 0;
        data.height = 0;
        data.channels = 0;
        data.path.clear();
    }
}

bool TextureImporter::GetTextureInfo(const std::string& path, int& width, int& height, int& channels)
{
    return stbi_info(path.c_str(), &width, &height, &channels) != 0;
}

bool TextureImporter::IsSupportedFormat(const std::string& path)
{
    // Convert to lowercase for case-insensitive comparison
    std::string lowerPath = path;
    std::transform(lowerPath.begin(), lowerPath.end(), lowerPath.begin(), ::tolower);
    
    // Check file extension
    return EndsWith(lowerPath, ".png") ||
           EndsWith(lowerPath, ".jpg") ||
           EndsWith(lowerPath, ".jpeg") ||
           EndsWith(lowerPath, ".bmp") ||
           EndsWith(lowerPath, ".tga") ||
           EndsWith(lowerPath, ".psd") ||
           EndsWith(lowerPath, ".gif") ||
           EndsWith(lowerPath, ".hdr") ||
           EndsWith(lowerPath, ".pic");
}

const char* TextureImporter::GetLastError()
{
    return stbi_failure_reason();
}

} // namespace Assets
