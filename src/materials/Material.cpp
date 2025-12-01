#include "Material.h"
#include "../core/Log.h"
#include <fstream>
#include <sstream>
#include <algorithm>

// Prevent Windows.h min/max macros from interfering
#undef min
#undef max

namespace Materials {

Material::Material(const std::string& name)
    : m_name(name)
    , m_properties()
    , m_albedoTexture(0)
    , m_normalTexture(0)
    , m_roughnessTexture(0)
    , m_metallicTexture(0)
    , m_aoTexture(0)
{
    CORE_LOG_INFO("Material created: {0}", name.c_str());
}

// ===== Texture Slots =====

void Material::SetAlbedoTexture(AssetID id) {
    m_albedoTexture = id;
    if (id != 0) {
        CORE_LOG_INFO("Material '{0}': Albedo texture set (AssetID: {1})", m_name.c_str(), id);
    }
}

void Material::SetNormalTexture(AssetID id) {
    m_normalTexture = id;
    if (id != 0) {
        CORE_LOG_INFO("Material '{0}': Normal texture set (AssetID: {1})", m_name.c_str(), id);
    }
}

void Material::SetRoughnessTexture(AssetID id) {
    m_roughnessTexture = id;
    if (id != 0) {
        CORE_LOG_INFO("Material '{0}': Roughness texture set (AssetID: {1})", m_name.c_str(), id);
    }
}

void Material::SetMetallicTexture(AssetID id) {
    m_metallicTexture = id;
    if (id != 0) {
        CORE_LOG_INFO("Material '{0}': Metallic texture set (AssetID: {1})", m_name.c_str(), id);
    }
}

void Material::SetAOTexture(AssetID id) {
    m_aoTexture = id;
    if (id != 0) {
        CORE_LOG_INFO("Material '{0}': AO texture set (AssetID: {1})", m_name.c_str(), id);
    }
}

// ===== Material Properties =====

void Material::SetAlbedoColor(float r, float g, float b, float a) {
    m_properties.albedoColor[0] = std::max(0.0f, std::min(1.0f, r));
    m_properties.albedoColor[1] = std::max(0.0f, std::min(1.0f, g));
    m_properties.albedoColor[2] = std::max(0.0f, std::min(1.0f, b));
    m_properties.albedoColor[3] = std::max(0.0f, std::min(1.0f, a));
}

void Material::SetMetallic(float value) {
    m_properties.metallic = std::max(0.0f, std::min(1.0f, value));
}

void Material::SetRoughness(float value) {
    m_properties.roughness = std::max(0.0f, std::min(1.0f, value));
}

// ===== Validation =====

bool Material::IsValid() const {
    // Material is valid if name is not empty
    return !m_name.empty();
}

// ===== Serialization (v2.1.0 H2.1) =====

bool Material::SaveToFile(const std::string& filepath) const
{
    std::ofstream file(filepath);
    if (!file.is_open())
    {
        CORE_LOG_ERROR("Material::SaveToFile: Failed to open file for writing: " + filepath);
        return false;
    }
    
    // Write JSON manually (no external JSON library dependency)
    file << "{\n";
    file << "  \"name\": \"" << m_name << "\",\n";
    file << "  \"version\": \"2.1.0\",\n";
    file << "  \"properties\": {\n";
    file << "    \"albedoColor\": [" 
         << m_properties.albedoColor[0] << ", " 
         << m_properties.albedoColor[1] << ", " 
         << m_properties.albedoColor[2] << ", " 
         << m_properties.albedoColor[3] << "],\n";
    file << "    \"metallic\": " << m_properties.metallic << ",\n";
    file << "    \"roughness\": " << m_properties.roughness << "\n";
    file << "  },\n";
    file << "  \"textures\": {\n";
    file << "    \"albedo\": " << m_albedoTexture << ",\n";
    file << "    \"normal\": " << m_normalTexture << ",\n";
    file << "    \"roughness\": " << m_roughnessTexture << ",\n";
    file << "    \"metallic\": " << m_metallicTexture << ",\n";
    file << "    \"ao\": " << m_aoTexture << "\n";
    file << "  }\n";
    file << "}\n";
    
    file.close();
    
    CORE_LOG_INFO("Material::SaveToFile: Saved material to: " + filepath);
    return true;
}

bool Material::LoadFromFile(const std::string& filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open())
    {
        CORE_LOG_ERROR("Material::LoadFromFile: Failed to open file for reading: " + filepath);
        return false;
    }
    
    // Read entire file into string
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    file.close();
    
    // Simple JSON parsing (manual, no external library)
    // This is a simplified parser for our specific JSON format
    
    // Parse name
    size_t namePos = content.find("\"name\":");
    if (namePos != std::string::npos)
    {
        size_t startQuote = content.find("\"", namePos + 7);
        size_t endQuote = content.find("\"", startQuote + 1);
        if (startQuote != std::string::npos && endQuote != std::string::npos)
        {
            m_name = content.substr(startQuote + 1, endQuote - startQuote - 1);
        }
    }
    
    // Parse albedoColor
    size_t albedoColorPos = content.find("\"albedoColor\":");
    if (albedoColorPos != std::string::npos)
    {
        size_t startBracket = content.find("[", albedoColorPos);
        size_t endBracket = content.find("]", startBracket);
        if (startBracket != std::string::npos && endBracket != std::string::npos)
        {
            std::string arrayStr = content.substr(startBracket + 1, endBracket - startBracket - 1);
            std::stringstream ss(arrayStr);
            char comma;
            ss >> m_properties.albedoColor[0] >> comma 
               >> m_properties.albedoColor[1] >> comma 
               >> m_properties.albedoColor[2] >> comma 
               >> m_properties.albedoColor[3];
        }
    }
    
    // Parse metallic
    size_t metallicPos = content.find("\"metallic\":");
    if (metallicPos != std::string::npos)
    {
        std::stringstream ss(content.substr(metallicPos + 11));
        ss >> m_properties.metallic;
    }
    
    // Parse roughness
    size_t roughnessPos = content.find("\"roughness\":");
    if (roughnessPos != std::string::npos)
    {
        std::stringstream ss(content.substr(roughnessPos + 12));
        ss >> m_properties.roughness;
    }
    
    // Parse texture IDs
    size_t texturesPos = content.find("\"textures\":");
    if (texturesPos != std::string::npos)
    {
        size_t albedoTexPos = content.find("\"albedo\":", texturesPos);
        if (albedoTexPos != std::string::npos)
        {
            std::stringstream ss(content.substr(albedoTexPos + 9));
            ss >> m_albedoTexture;
        }
        
        size_t normalTexPos = content.find("\"normal\":", texturesPos);
        if (normalTexPos != std::string::npos)
        {
            std::stringstream ss(content.substr(normalTexPos + 9));
            ss >> m_normalTexture;
        }
        
        size_t roughnessTexPos = content.find("\"roughness\":", texturesPos);
        if (roughnessTexPos != std::string::npos)
        {
            std::stringstream ss(content.substr(roughnessTexPos + 12));
            ss >> m_roughnessTexture;
        }
        
        size_t metallicTexPos = content.find("\"metallic\":", texturesPos);
        if (metallicTexPos != std::string::npos)
        {
            std::stringstream ss(content.substr(metallicTexPos + 11));
            ss >> m_metallicTexture;
        }
        
        size_t aoTexPos = content.find("\"ao\":", texturesPos);
        if (aoTexPos != std::string::npos)
        {
            std::stringstream ss(content.substr(aoTexPos + 5));
            ss >> m_aoTexture;
        }
    }
    
    CORE_LOG_INFO("Material::LoadFromFile: Loaded material from: " + filepath);
    CORE_LOG_INFO("  Name: " + m_name);
    CORE_LOG_INFO("  Metallic: " + std::to_string(m_properties.metallic));
    CORE_LOG_INFO("  Roughness: " + std::to_string(m_properties.roughness));
    
    return true;
}

} // namespace Materials
