# stb_image - Image Loading Library

**Version**: Latest (from https://github.com/nothings/stb)  
**License**: Public Domain (or MIT)  
**Author**: Sean Barrett

---

## Description

`stb_image` is a single-header C library for loading images in various formats (PNG, JPG, BMP, TGA, PSD, GIF, HDR, PIC) into memory as pixel data.

---

## Supported Formats

| Format | Extension | Notes |
|--------|-----------|-------|
| PNG    | `.png`    | Full support (8-bit, 16-bit, paletted) |
| JPG    | `.jpg`, `.jpeg` | Baseline & progressive JPEG |
| BMP    | `.bmp`    | Non-RLE only |
| TGA    | `.tga`    | Truevision Targa |
| PSD    | `.psd`    | Photoshop (composited view only) |
| GIF    | `.gif`    | Animated GIF support |
| HDR    | `.hdr`    | Radiance RGBE |
| PIC    | `.pic`    | Softimage PIC |

---

## Usage

### In Your Code

```cpp
#include "external/stb/stb_image.h"

// Load image
int width, height, channels;
unsigned char* data = stbi_load("texture.png", &width, &height, &channels, 0);

if (data) {
    // Use image data (RGBA format if channels == 4)
    // ...
    
    // Free memory when done
    stbi_image_free(data);
} else {
    // Handle error
    const char* reason = stbi_failure_reason();
}
```

### Force RGBA Format

```cpp
// Force 4 channels (RGBA) regardless of source format
unsigned char* data = stbi_load("texture.png", &width, &height, &channels, 4);
```

---

## Integration in Project

### Files

- `stb_image.h` - Header file (single-header library)
- `stb_image_impl.cpp` - Implementation file (contains `#define STB_IMAGE_IMPLEMENTATION`)

### CMakeLists.txt

```cmake
# Include stb_image implementation
target_sources(ImagineStudio PRIVATE external/stb/stb_image_impl.cpp)
target_include_directories(ImagineStudio PRIVATE external/stb)
```

### Visual Studio Project

Add to `Imagine Studio.vcxproj`:
```xml
<ClCompile Include="external\stb\stb_image_impl.cpp" />
```

---

## API Reference

### Main Functions

| Function | Description |
|----------|-------------|
| `stbi_load()` | Load image from file |
| `stbi_load_from_memory()` | Load image from memory buffer |
| `stbi_image_free()` | Free loaded image data |
| `stbi_failure_reason()` | Get error message string |
| `stbi_info()` | Get image dimensions without loading |
| `stbi_set_flip_vertically_on_load()` | Flip image vertically during load |

### Example: Info Without Loading

```cpp
int width, height, channels;
if (stbi_info("texture.png", &width, &height, &channels)) {
    printf("Image: %dx%d, %d channels\n", width, height, channels);
}
```

### Example: Flip Vertically (for OpenGL/DirectX)

```cpp
// DirectX uses top-left origin, so flip if needed
stbi_set_flip_vertically_on_load(true);
unsigned char* data = stbi_load("texture.png", &width, &height, &channels, 4);
```

---

## Notes

- **Thread-safe**: `stbi_load()` is thread-safe (no global state)
- **Memory**: Caller must call `stbi_image_free()` to release memory
- **Channels**: 
  - `1` = Grey
  - `2` = Grey + Alpha
  - `3` = RGB
  - `4` = RGBA
- **16-bit support**: Use `stbi_load_16()` for 16-bit per channel images

---

## Links

- **Repository**: https://github.com/nothings/stb
- **Documentation**: https://github.com/nothings/stb/blob/master/stb_image.h (header comments)
- **License**: Public Domain / MIT

---

**Integrated**: 2025-01-21 (Sprint v1.9.0 - H2.1)  
**Used in**: `src/assets/TextureImporter.cpp` (H2.2)
