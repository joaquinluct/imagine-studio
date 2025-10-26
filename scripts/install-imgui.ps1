# ImGui Installation Script for Imagine Studio
# Downloads and integrates ImGui from GitHub (supports branches and releases)

param(
    [string]$Version = "latest",  # "latest" or specific version like "v1.91.5"
    [string]$Branch = "docking",   # Branch name (e.g., "docking", "master") - takes priority over Version
    [switch]$Reinstall = $false    # Force reinstall even if already exists
)

# Configuration
$ImGuiDir = "external/imgui"
$ImGuiRepo = "https://github.com/ocornut/imgui"
$TempDir = "temp_imgui_download"

Write-Host "================================" -ForegroundColor Cyan
Write-Host "ImGui Installation Script" -ForegroundColor Cyan
Write-Host "================================" -ForegroundColor Cyan
Write-Host ""

# Check if ImGui already exists
if ((Test-Path $ImGuiDir) -and -not $Reinstall) {
    Write-Host "ImGui already exists at '$ImGuiDir'." -ForegroundColor Yellow
    Write-Host "Use -Reinstall flag to force reinstallation." -ForegroundColor Yellow
    exit 0
}

# Remove existing ImGui if reinstalling
if ((Test-Path $ImGuiDir) -and $Reinstall) {
    Write-Host "Removing existing ImGui directory..." -ForegroundColor Yellow
    Remove-Item -Path $ImGuiDir -Recurse -Force
}

# Create external directory if it doesn't exist
if (-not (Test-Path "external")) {
    Write-Host "Creating 'external' directory..." -ForegroundColor Green
    New-Item -ItemType Directory -Path "external" | Out-Null
}

# Determine download source (branch takes priority over version)
if ($Branch) {
    # Download from branch
    Write-Host "Using branch: $Branch" -ForegroundColor Green
    $DownloadUrl = "$ImGuiRepo/archive/refs/heads/$Branch.zip"
    $VersionTag = "branch-$Branch"
} else {
    # Download from release tag
    if ($Version -eq "latest") {
        Write-Host "Fetching latest ImGui release version..." -ForegroundColor Cyan
        try {
            $latestRelease = Invoke-RestMethod -Uri "$ImGuiRepo/releases/latest" -Headers @{ "User-Agent" = "PowerShell" }
            $Version = $latestRelease.tag_name
            Write-Host "Latest version: $Version" -ForegroundColor Green
        } catch {
            Write-Host "Failed to fetch latest release. Using default v1.91.5" -ForegroundColor Yellow
            $Version = "v1.91.5"
        }
    } else {
        Write-Host "Using specified version: $Version" -ForegroundColor Green
    }
    $DownloadUrl = "$ImGuiRepo/archive/refs/tags/$Version.zip"
    $VersionTag = $Version
}

$ZipFile = "$TempDir.zip"

Write-Host ""
Write-Host "Downloading ImGui from GitHub..." -ForegroundColor Cyan
Write-Host "Source: $(if ($Branch) { "Branch '$Branch'" } else { "Release '$Version'" })" -ForegroundColor Gray
Write-Host "URL: $DownloadUrl" -ForegroundColor Gray

try {
    # Download ImGui zip
    Invoke-WebRequest -Uri $DownloadUrl -OutFile $ZipFile -UseBasicParsing
    Write-Host "Download completed: $ZipFile" -ForegroundColor Green
    
    # Extract zip
    Write-Host "Extracting ImGui..." -ForegroundColor Cyan
    Expand-Archive -Path $ZipFile -DestinationPath $TempDir -Force
    
    # Find extracted directory (format: imgui-<version_without_v>)
    $ExtractedDir = Get-ChildItem -Path $TempDir -Directory | Select-Object -First 1
    
    if (-not $ExtractedDir) {
        throw "Failed to find extracted ImGui directory"
    }
    
    Write-Host "Extracted to: $($ExtractedDir.FullName)" -ForegroundColor Green
    
    # Create destination directory
    New-Item -ItemType Directory -Path $ImGuiDir -Force | Out-Null
    
    # Copy core files
    Write-Host ""
    Write-Host "Copying ImGui core files..." -ForegroundColor Cyan
    
    $CoreFiles = @(
        "imgui.h",
        "imgui.cpp",
        "imgui_demo.cpp",
        "imgui_draw.cpp",
        "imgui_tables.cpp",
        "imgui_widgets.cpp",
        "imgui_internal.h",
        "imconfig.h",
        "imstb_rectpack.h",
        "imstb_textedit.h",
        "imstb_truetype.h"
    )
    
    foreach ($file in $CoreFiles) {
        $srcFile = Join-Path $ExtractedDir.FullName $file
        if (Test-Path $srcFile) {
            Copy-Item -Path $srcFile -Destination $ImGuiDir
            Write-Host "  ? $file" -ForegroundColor Green
        } else {
            Write-Host "  ? $file not found (skipped)" -ForegroundColor Yellow
        }
    }
    
    # Copy backend files (DX12 + Win32)
    Write-Host ""
    Write-Host "Copying ImGui backends (DX12 + Win32)..." -ForegroundColor Cyan
    
    $BackendsDir = Join-Path $ExtractedDir.FullName "backends"
    $DestBackendsDir = Join-Path $ImGuiDir "backends"
    New-Item -ItemType Directory -Path $DestBackendsDir -Force | Out-Null
    
    $BackendFiles = @(
        "imgui_impl_dx12.h",
        "imgui_impl_dx12.cpp",
        "imgui_impl_win32.h",
        "imgui_impl_win32.cpp"
    )
    
    foreach ($file in $BackendFiles) {
        $srcFile = Join-Path $BackendsDir $file
        if (Test-Path $srcFile) {
            Copy-Item -Path $srcFile -Destination $DestBackendsDir
            Write-Host "  ? backends/$file" -ForegroundColor Green
        } else {
            Write-Host "  ? backends/$file not found (skipped)" -ForegroundColor Yellow
        }
    }
    
    # Create version file for tracking
    Write-Host ""
    Write-Host "Creating version tracking file..." -ForegroundColor Cyan
    $VersionFile = Join-Path $ImGuiDir "VERSION.txt"
    @"
ImGui Version: $VersionTag
Downloaded: $(Get-Date -Format "yyyy-MM-dd HH:mm:ss")
Source: $ImGuiRepo
Download Type: $(if ($Branch) { "Branch: $Branch" } else { "Release: $Version" })
"@ | Out-File -FilePath $VersionFile -Encoding UTF8
    Write-Host "  ? VERSION.txt created" -ForegroundColor Green
    
    # Cleanup
    Write-Host ""
    Write-Host "Cleaning up temporary files..." -ForegroundColor Cyan
    Remove-Item -Path $ZipFile -Force
    Remove-Item -Path $TempDir -Recurse -Force
    Write-Host "  ? Cleanup completed" -ForegroundColor Green
    
    # Summary
    Write-Host ""
    Write-Host "================================" -ForegroundColor Cyan
    Write-Host "Installation Summary" -ForegroundColor Cyan
    Write-Host "================================" -ForegroundColor Cyan
    Write-Host "ImGui Version: $VersionTag" -ForegroundColor Green
    Write-Host "Download Type: $(if ($Branch) { "Branch '$Branch'" } else { "Release '$Version'" })" -ForegroundColor Green
    Write-Host "Installation Path: $ImGuiDir" -ForegroundColor Green
    Write-Host "Core Files: $($CoreFiles.Count) files" -ForegroundColor Green
    Write-Host "Backend Files: $($BackendFiles.Count) files (DX12 + Win32)" -ForegroundColor Green
    Write-Host ""
    Write-Host "? ImGui installation completed successfully!" -ForegroundColor Green
    Write-Host ""
    Write-Host "Next steps:" -ForegroundColor Cyan
    Write-Host "  1. Run CMake configuration: cmake -S . -B build" -ForegroundColor Gray
    Write-Host "  2. Build project: cmake --build build --config Debug" -ForegroundColor Gray
    Write-Host "  3. Continue with Sprint v1.3.0 Task H1.2" -ForegroundColor Gray
    Write-Host ""
    
} catch {
    Write-Host ""
    Write-Host "Error during installation:" -ForegroundColor Red
    Write-Host $_.Exception.Message -ForegroundColor Red
    Write-Host ""
    
    # Cleanup on error
    if (Test-Path $ZipFile) { Remove-Item -Path $ZipFile -Force }
    if (Test-Path $TempDir) { Remove-Item -Path $TempDir -Recurse -Force }
    
    exit 1
}
