#pragma once

#include <string>

namespace Editor {

/**
 * @brief Asset Browser panel for navigating and managing project assets
 * 
 * **Version**: v1.9.0 H4.1 - Placeholder implementation
 * 
 * Displays hardcoded folder structure:
 * - assets/
 *   - textures/
 *   - meshes/
 *   - shaders/
 *   - scenes/
 * 
 * Future iterations (H4.2-H4.4) will add:
 * - Real filesystem scanning (when migrating to C++17)
 * - Thumbnails for textures
 * - Drag & drop functionality
 * - Context menus (Reimport, Delete, Properties)
 * 
 * Part of Sprint v1.9.0 - Asset System & Resource Management
 */
class AssetBrowser {
public:
    AssetBrowser();
    ~AssetBrowser();

    /**
     * @brief Render the Asset Browser panel with ImGui
     */
    void Render();

    /**
     * @brief Get the currently selected asset name
     * @return Name of selected asset, empty if none selected
     */
    const std::string& GetSelectedAsset() const { return m_selectedAsset; }

    /**
     * @brief Check if an asset is currently selected
     * @return true if asset selected, false otherwise
     */
    bool HasSelection() const { return !m_selectedAsset.empty(); }

private:
    /**
     * @brief Render folder tree view (left side panel)
     * Hardcoded structure for H4.1
     */
    void RenderFolderTree();

    /**
     * @brief Render asset grid view (right side panel)
     * Shows placeholder assets based on selected folder
     */
    void RenderAssetGrid();

    /**
     * @brief Render a single folder node in tree view
     * @param folderName Folder name to display
     * @param isLeaf true if folder has no subdirectories
     */
    void RenderFolderNode(const char* folderName, bool isLeaf = false);

    /**
     * @brief Render a single asset item in grid view
     * @param assetName Asset name
     * @param extension File extension (for icon color)
     * @param index Item index (for layout)
     */
    void RenderAssetItem(const char* assetName, const char* extension, int index);

private:
    std::string m_currentFolder;       ///< Current folder being viewed
    std::string m_selectedAsset;       ///< Currently selected asset name
    
    float m_thumbnailSize = 80.0f;     ///< Size of asset thumbnails/icons in grid
    float m_padding = 10.0f;           ///< Padding between grid items
};

} // namespace Editor
