#include "AssetBrowser.h"
#include <imgui.h>

namespace Editor {

AssetBrowser::AssetBrowser()
    : m_currentFolder("assets/")
{
}

AssetBrowser::~AssetBrowser()
{
}

void AssetBrowser::Render()
{
    ImGui::Begin("Asset Browser");

    // Split panel: Folder tree (left) | Asset grid (right)
    ImGui::Columns(2, "AssetBrowserColumns", true);

    // Left panel: Folder tree
    ImGui::BeginChild("FolderTree", ImVec2(0, 0), true);
    RenderFolderTree();
    ImGui::EndChild();

    ImGui::NextColumn();

    // Right panel: Asset grid
    ImGui::BeginChild("AssetGrid", ImVec2(0, 0), true);
    
    // Current directory label
    ImGui::Text("Current Directory: %s", m_currentFolder.c_str());
    ImGui::Separator();

    RenderAssetGrid();
    
    ImGui::EndChild();

    ImGui::Columns(1);

    ImGui::End();
}

void AssetBrowser::RenderFolderTree()
{
    ImGui::Text("Folders");
    ImGui::Separator();

    // Hardcoded folder structure (v1.9.0 H4.1 placeholder)
    // Root folder
    ImGuiTreeNodeFlags rootFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
    bool rootOpen = ImGui::TreeNodeEx("assets/", rootFlags);
    
    if (ImGui::IsItemClicked()) {
        m_currentFolder = "assets/";
        m_selectedAsset.clear();
    }
    
    if (rootOpen) {
        // Subfolder: textures/
        RenderFolderNode("textures/", true);
        
        // Subfolder: meshes/
        RenderFolderNode("meshes/", true);
        
        // Subfolder: shaders/
        RenderFolderNode("shaders/", true);
        
        // Subfolder: scenes/
        RenderFolderNode("scenes/", true);
        
        ImGui::TreePop();
    }
}

void AssetBrowser::RenderFolderNode(const char* folderName, bool isLeaf)
{
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
    
    // Leaf folders (no subdirectories)
    if (isLeaf) {
        flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
    }
    
    // Highlight if this is the current folder
    std::string fullPath = "assets/";
    fullPath += folderName;
    
    if (m_currentFolder == fullPath) {
        flags |= ImGuiTreeNodeFlags_Selected;
    }
    
    ImGui::TreeNodeEx(folderName, flags);
    
    // Click to select directory
    if (ImGui::IsItemClicked()) {
        m_currentFolder = fullPath;
        m_selectedAsset.clear();
    }
}

void AssetBrowser::RenderAssetGrid()
{
    // Hardcoded placeholder assets based on current folder (v1.9.0 H4.1)
    
    if (m_currentFolder == "assets/") {
        ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "Select a folder to view assets.");
        return;
    }
    
    // Calculate grid layout
    float panelWidth = ImGui::GetContentRegionAvail().x;
    int columnCount = static_cast<int>(panelWidth / (m_thumbnailSize + m_padding));
    if (columnCount < 1) columnCount = 1;
    
    int assetCount = 0;
    
    // Placeholder assets per folder
    if (m_currentFolder == "assets/textures/") {
        RenderAssetItem("test_4x4", ".png", assetCount++);
        if (assetCount % columnCount != 0) ImGui::SameLine();
        RenderAssetItem("default_white", ".png", assetCount++);
        if (assetCount % columnCount != 0) ImGui::SameLine();
        RenderAssetItem("checker_pattern", ".png", assetCount++);
    }
    else if (m_currentFolder == "assets/meshes/") {
        RenderAssetItem("test_triangle", ".obj", assetCount++);
        if (assetCount % columnCount != 0) ImGui::SameLine();
        RenderAssetItem("test_quad", ".obj", assetCount++);
        if (assetCount % columnCount != 0) ImGui::SameLine();
        RenderAssetItem("test_cube", ".obj", assetCount++);
    }
    else if (m_currentFolder == "assets/shaders/") {
        RenderAssetItem("basic_vertex", ".hlsl", assetCount++);
        if (assetCount % columnCount != 0) ImGui::SameLine();
        RenderAssetItem("basic_pixel", ".hlsl", assetCount++);
    }
    else if (m_currentFolder == "assets/scenes/") {
        ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "No scenes yet. Create one in Scene menu.");
    }
}

void AssetBrowser::RenderAssetItem(const char* assetName, const char* extension, int index)
{
    // Unique ID for ImGui
    ImGui::PushID(index);

    ImGui::BeginGroup();

    // Asset thumbnail/icon (colored rectangle simulating texture preview)
    ImVec4 thumbnailColor(0.3f, 0.3f, 0.3f, 1.0f);
    
    // Color by type (simulating texture preview)
    if (extension[1] == 'p' && extension[2] == 'n' && extension[3] == 'g') {
        // Blue gradient for textures (simulating image preview)
        thumbnailColor = ImVec4(0.2f, 0.5f, 0.8f, 1.0f);
    } 
    else if (extension[1] == 'o' && extension[2] == 'b' && extension[3] == 'j') {
        // Purple for meshes (wireframe icon)
        thumbnailColor = ImVec4(0.5f, 0.2f, 0.8f, 1.0f);
    } 
    else if (extension[1] == 'h' && extension[2] == 'l' && extension[3] == 's') {
        // Orange for shaders (code icon)
        thumbnailColor = ImVec4(0.8f, 0.5f, 0.2f, 1.0f);
    }

    // Draw thumbnail as colored rectangle (simulating image preview)
    ImVec2 thumbnailMin = ImGui::GetCursorScreenPos();
    ImVec2 thumbnailMax(thumbnailMin.x + m_thumbnailSize, thumbnailMin.y + m_thumbnailSize);
    
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    
    // Draw filled rectangle (thumbnail background)
    drawList->AddRectFilled(thumbnailMin, thumbnailMax, ImGui::ColorConvertFloat4ToU32(thumbnailColor));
    
    // Draw border (darker)
    ImVec4 borderColor(thumbnailColor.x * 0.5f, thumbnailColor.y * 0.5f, thumbnailColor.z * 0.5f, 1.0f);
    drawList->AddRect(thumbnailMin, thumbnailMax, ImGui::ColorConvertFloat4ToU32(borderColor), 0.0f, 0, 2.0f);
    
    // Invisible button for interaction (same size as thumbnail)
    ImGui::InvisibleButton("##thumbnail", ImVec2(m_thumbnailSize, m_thumbnailSize));
    
    bool clicked = ImGui::IsItemClicked();
    bool hovered = ImGui::IsItemHovered();

    // Hover effect (lighter border)
    if (hovered) {
        ImVec4 hoverColor(1.0f, 1.0f, 1.0f, 0.5f);
        drawList->AddRect(thumbnailMin, thumbnailMax, ImGui::ColorConvertFloat4ToU32(hoverColor), 0.0f, 0, 3.0f);
    }

    // Selection highlight (yellow border)
    if (m_selectedAsset == assetName) {
        drawList->AddRect(thumbnailMin, thumbnailMax, IM_COL32(255, 255, 0, 255), 0.0f, 0, 3.0f);
    }

    // Click to select
    if (clicked) {
        m_selectedAsset = assetName;
    }

    // Filename label (centered, truncated if too long)
    ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + m_thumbnailSize);
    ImGui::TextWrapped("%s", assetName);
    ImGui::PopTextWrapPos();

    ImGui::EndGroup();

    ImGui::PopID();
}

} // namespace Editor
