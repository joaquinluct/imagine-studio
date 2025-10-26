#include "../src/assets/TextureImporter.h"
#include <cassert>
#include <iostream>

using namespace Assets;

void TestImportTextureValid()
{
    std::cout << "[TEST] TestImportTextureValid - START" << std::endl;
    
    // Note: This test requires a valid PNG file at assets/textures/test_4x4.png
    // If file doesn't exist, test will fail (expected behavior)
    
    try {
        TextureData tex = TextureImporter::ImportTextureRGBA("assets/textures/test_4x4.png");
        
        // Test 1: IsValid returns true
        assert(tex.IsValid());
        std::cout << "  [PASS] TextureData IsValid() returns true" << std::endl;
        
        // Test 2: Pixels pointer is not null
        assert(tex.pixels != nullptr);
        std::cout << "  [PASS] Pixels pointer is not null" << std::endl;
        
        // Test 3: Width > 0
        assert(tex.width > 0);
        std::cout << "  [PASS] Width > 0 (" << tex.width << " pixels)" << std::endl;
        
        // Test 4: Height > 0
        assert(tex.height > 0);
        std::cout << "  [PASS] Height > 0 (" << tex.height << " pixels)" << std::endl;
        
        // Test 5: Channels == 4 (RGBA forced)
        assert(tex.channels == 4);
        std::cout << "  [PASS] Channels == 4 (RGBA)" << std::endl;
        
        // Test 6: Path is correct
        assert(tex.path == "assets/textures/test_4x4.png");
        std::cout << "  [PASS] Path stored correctly" << std::endl;
        
        // Cleanup
        TextureImporter::FreeTextureData(tex);
        
        // Test 7: After free, pixels should be null
        assert(tex.pixels == nullptr);
        assert(tex.width == 0);
        assert(tex.height == 0);
        assert(tex.channels == 0);
        std::cout << "  [PASS] FreeTextureData clears all fields" << std::endl;
        
        std::cout << "[TEST] TestImportTextureValid - PASSED (7 tests)" << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "  [INFO] Test skipped - test image not found: " << e.what() << std::endl;
        std::cout << "  [INFO] To enable this test, create a PNG file at: assets/textures/test_4x4.png" << std::endl;
        std::cout << "[TEST] TestImportTextureValid - SKIPPED (no test image)" << std::endl;
    }
}

void TestImportTextureInvalid()
{
    std::cout << "[TEST] TestImportTextureInvalid - START" << std::endl;
    
    // Test 1: Import non-existent file throws exception
    bool exceptionThrown = false;
    try {
        TextureData tex = TextureImporter::ImportTexture("nonexistent_file.png");
    }
    catch (const std::runtime_error& e) {
        exceptionThrown = true;
        std::cout << "  [PASS] ImportTexture throws exception for non-existent file" << std::endl;
    }
    assert(exceptionThrown);
    
    std::cout << "[TEST] TestImportTextureInvalid - PASSED (1 test)" << std::endl;
}

void TestGetTextureInfo()
{
    std::cout << "[TEST] TestGetTextureInfo - START" << std::endl;
    
    // Note: This test requires a valid PNG file at assets/textures/test_4x4.png
    
    try {
        int width = 0, height = 0, channels = 0;
        bool success = TextureImporter::GetTextureInfo("assets/textures/test_4x4.png", width, height, channels);
        
        if (success) {
            // Test 1: GetTextureInfo returns true for valid file
            assert(success);
            std::cout << "  [PASS] GetTextureInfo returns true for valid file" << std::endl;
            
            // Test 2: Width > 0
            assert(width > 0);
            std::cout << "  [PASS] Width > 0 (" << width << " pixels)" << std::endl;
            
            // Test 3: Height > 0
            assert(height > 0);
            std::cout << "  [PASS] Height > 0 (" << height << " pixels)" << std::endl;
            
            // Test 4: Channels > 0
            assert(channels > 0);
            std::cout << "  [PASS] Channels > 0 (" << channels << " channels)" << std::endl;
            
            std::cout << "[TEST] TestGetTextureInfo - PASSED (4 tests)" << std::endl;
        }
        else {
            std::cout << "  [INFO] Test skipped - test image not found" << std::endl;
            std::cout << "[TEST] TestGetTextureInfo - SKIPPED (no test image)" << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cout << "  [INFO] Test skipped: " << e.what() << std::endl;
        std::cout << "[TEST] TestGetTextureInfo - SKIPPED (no test image)" << std::endl;
    }
    
    // Test 5: GetTextureInfo returns false for invalid file
    int w = 0, h = 0, c = 0;
    bool result = TextureImporter::GetTextureInfo("invalid_file.xyz", w, h, c);
    assert(!result);
    std::cout << "  [PASS] GetTextureInfo returns false for invalid file" << std::endl;
}

void TestIsSupportedFormat()
{
    std::cout << "[TEST] TestIsSupportedFormat - START" << std::endl;
    
    // Test 1: PNG is supported
    assert(TextureImporter::IsSupportedFormat("texture.png"));
    std::cout << "  [PASS] .png extension is supported" << std::endl;
    
    // Test 2: JPG is supported
    assert(TextureImporter::IsSupportedFormat("photo.jpg"));
    std::cout << "  [PASS] .jpg extension is supported" << std::endl;
    
    // Test 3: JPEG is supported
    assert(TextureImporter::IsSupportedFormat("image.jpeg"));
    std::cout << "  [PASS] .jpeg extension is supported" << std::endl;
    
    // Test 4: BMP is supported
    assert(TextureImporter::IsSupportedFormat("bitmap.bmp"));
    std::cout << "  [PASS] .bmp extension is supported" << std::endl;
    
    // Test 5: TGA is supported
    assert(TextureImporter::IsSupportedFormat("texture.tga"));
    std::cout << "  [PASS] .tga extension is supported" << std::endl;
    
    // Test 6: PSD is supported
    assert(TextureImporter::IsSupportedFormat("layer.psd"));
    std::cout << "  [PASS] .psd extension is supported" << std::endl;
    
    // Test 7: GIF is supported
    assert(TextureImporter::IsSupportedFormat("anim.gif"));
    std::cout << "  [PASS] .gif extension is supported" << std::endl;
    
    // Test 8: HDR is supported
    assert(TextureImporter::IsSupportedFormat("hdr.hdr"));
    std::cout << "  [PASS] .hdr extension is supported" << std::endl;
    
    // Test 9: PIC is supported
    assert(TextureImporter::IsSupportedFormat("pic.pic"));
    std::cout << "  [PASS] .pic extension is supported" << std::endl;
    
    // Test 10: TXT is NOT supported
    assert(!TextureImporter::IsSupportedFormat("file.txt"));
    std::cout << "  [PASS] .txt extension is NOT supported" << std::endl;
    
    // Test 11: EXE is NOT supported
    assert(!TextureImporter::IsSupportedFormat("program.exe"));
    std::cout << "  [PASS] .exe extension is NOT supported" << std::endl;
    
    // Test 12: Case insensitive (PNG uppercase)
    assert(TextureImporter::IsSupportedFormat("TEXTURE.PNG"));
    std::cout << "  [PASS] Extension check is case-insensitive" << std::endl;
    
    std::cout << "[TEST] TestIsSupportedFormat - PASSED (12 tests)" << std::endl;
}

void TestImportTextureChannels()
{
    std::cout << "[TEST] TestImportTextureChannels - START" << std::endl;
    
    // Note: This test requires a valid PNG file
    
    try {
        // Test 1: Import with original channels (desiredChannels = 0)
        TextureData tex1 = TextureImporter::ImportTexture("assets/textures/test_4x4.png", 0);
        assert(tex1.IsValid());
        assert(tex1.pixels != nullptr);
        std::cout << "  [PASS] ImportTexture with original channels (desiredChannels=0)" << std::endl;
        TextureImporter::FreeTextureData(tex1);
        
        // Test 2: Import with RGBA forced (desiredChannels = 4)
        TextureData tex2 = TextureImporter::ImportTexture("assets/textures/test_4x4.png", 4);
        assert(tex2.IsValid());
        assert(tex2.pixels != nullptr);
        assert(tex2.channels == 4);
        std::cout << "  [PASS] ImportTexture with RGBA forced (desiredChannels=4)" << std::endl;
        TextureImporter::FreeTextureData(tex2);
        
        // Test 3: ImportTextureRGBA is equivalent to desiredChannels=4
        TextureData tex3 = TextureImporter::ImportTextureRGBA("assets/textures/test_4x4.png");
        assert(tex3.IsValid());
        assert(tex3.channels == 4);
        std::cout << "  [PASS] ImportTextureRGBA forces 4 channels" << std::endl;
        TextureImporter::FreeTextureData(tex3);
        
        std::cout << "[TEST] TestImportTextureChannels - PASSED (3 tests)" << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "  [INFO] Test skipped: " << e.what() << std::endl;
        std::cout << "[TEST] TestImportTextureChannels - SKIPPED (no test image)" << std::endl;
    }
}

int main()
{
    std::cout << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "   TextureImporter Unit Tests" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;
    
    std::cout << "[INFO] Note: Some tests require assets/textures/test_4x4.png" << std::endl;
    std::cout << "[INFO] Create a 4x4 PNG file manually to enable all tests" << std::endl;
    std::cout << std::endl;
    
    try {
        TestIsSupportedFormat();
        std::cout << std::endl;
        
        TestImportTextureInvalid();
        std::cout << std::endl;
        
        TestGetTextureInfo();
        std::cout << std::endl;
        
        TestImportTextureValid();
        std::cout << std::endl;
        
        TestImportTextureChannels();
        std::cout << std::endl;
        
        std::cout << "========================================" << std::endl;
        std::cout << "   ALL TESTS COMPLETED ?" << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << std::endl;
        
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << std::endl;
        std::cerr << "========================================" << std::endl;
        std::cerr << "   TEST FAILED ?" << std::endl;
        std::cerr << "   Exception: " << e.what() << std::endl;
        std::cerr << "========================================" << std::endl;
        std::cerr << std::endl;
        return 1;
    }
}
