cmake_minimum_required(VERSION 3.15)
project(ImagineStudio VERSION 1.0.0 LANGUAGES CXX)

# Force x64 architecture (required for DirectX 12 and publishing)
if(MSVC)
    set(CMAKE_GENERATOR_PLATFORM x64)
    # Ensure libraries link as x64
    link_directories("C:/Program Files (x86)/Windows Kits/10/Lib/10.0.26100.0/um/x64")
    link_directories("C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.44.35207/lib/x64")
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Prefer no compiler extensions so behavior is standard-conforming
set(CMAKE_CXX_EXTENSIONS OFF)

# Collect sources
file(GLOB_RECURSE SRC_FILES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.cxx"
    "${CMAKE_SOURCE_DIR}/src/*.cc"
    "${CMAKE_SOURCE_DIR}/src/*/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*/*/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*/*/*/*.cpp"
)

# Exclude test sources from the main executable sources
file(GLOB_RECURSE TEST_SRC_EXCLUDE "${CMAKE_SOURCE_DIR}/src/tests/*.cpp")
if (TEST_SRC_EXCLUDE)
    list(REMOVE_ITEM SRC_FILES ${TEST_SRC_EXCLUDE})
endif()

# Exclude example sources from the main executable sources (they build as separate targets)
file(GLOB_RECURSE EXAMPLE_SRC_EXCLUDE "${CMAKE_SOURCE_DIR}/src/examples/*.cpp")
if (EXAMPLE_SRC_EXCLUDE)
    list(REMOVE_ITEM SRC_FILES ${EXAMPLE_SRC_EXCLUDE})
endif()

# Examples
set(EXAMPLE_RUNNER_SRC "${CMAKE_SOURCE_DIR}/src/examples/ExampleRunner.cpp")
if (EXISTS ${EXAMPLE_RUNNER_SRC})
    add_executable(example_runner ${EXAMPLE_RUNNER_SRC})
    target_include_directories(example_runner PRIVATE ${CMAKE_SOURCE_DIR}/src)
    target_compile_definitions(example_runner PRIVATE BUILD_EXAMPLE_EXE)
    if (MSVC)
        target_link_libraries(example_runner PRIVATE Msimg32)
    endif()
endif()

# Adapter factory test
set(ADAPTER_TEST_FILE "${CMAKE_SOURCE_DIR}/src/tests/adapter_test.cpp")
if (EXISTS ${ADAPTER_TEST_FILE})
    add_executable(adapter_test ${ADAPTER_TEST_FILE})
    target_include_directories(adapter_test PRIVATE ${CMAKE_SOURCE_DIR}/src)
    # For adapter_test only include the AdapterFactory implementation (it returns a stub when BUILD_TEST_EXE)
    file(GLOB ADAPTER_SRC "${CMAKE_SOURCE_DIR}/src/renderer/AdapterFactory.cpp")
    target_sources(adapter_test PRIVATE ${ADAPTER_SRC})
    target_compile_definitions(adapter_test PRIVATE BUILD_TEST_EXE)
    if (MSVC)
        target_link_libraries(adapter_test PRIVATE Msimg32)
    endif()
endif()

file(GLOB_RECURSE ASSET_SRC "${CMAKE_SOURCE_DIR}/src/assets/*.cpp")
list(APPEND SRC_FILES ${ASSET_SRC})

# Ensure renderer sources are included (explicit glob)
file(GLOB_RECURSE RENDER_SRC "${CMAKE_SOURCE_DIR}/src/renderer/*.cpp")
list(APPEND SRC_FILES ${RENDER_SRC})

# Include tools sources (Profiler) so main exe links profiler
file(GLOB_RECURSE TOOLS_SRC "${CMAKE_SOURCE_DIR}/src/tools/*.cpp")
list(APPEND SRC_FILES ${TOOLS_SRC})

# Ensure assets VFS is included
list(APPEND SRC_FILES "${CMAKE_SOURCE_DIR}/src/assets/VFS.cpp")

# Ensure UI sources are included
file(GLOB_RECURSE UI_SRC "${CMAKE_SOURCE_DIR}/src/ui/*.cpp")
list(APPEND SRC_FILES ${UI_SRC})

# Ensure Editor UI sources are included (H4.1 - Editor Panels)
file(GLOB_RECURSE EDITOR_SRC "${CMAKE_SOURCE_DIR}/src/editor/*.cpp")
list(APPEND SRC_FILES ${EDITOR_SRC})

# Ensure Scene system sources are included (H1 - Entity System Core)
file(GLOB_RECURSE SCENE_SRC "${CMAKE_SOURCE_DIR}/src/scene/*.cpp")
list(APPEND SRC_FILES ${SCENE_SRC})

# Explicitly ensure renderer stubs added by recent iterations are included
list(APPEND SRC_FILES "${CMAKE_SOURCE_DIR}/src/renderer/Fence.cpp")
# Ensure TaskGraph is included
list(APPEND SRC_FILES "${CMAKE_SOURCE_DIR}/src/jobs/TaskGraph.cpp")

add_executable(ImagineStudio WIN32 ${SRC_FILES})

# Allow control over whether the window starts maximized (default ON)
option(IMAGINE_START_MAXIMIZED "Start the main window maximized" ON)

target_include_directories(ImagineStudio PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Propagate project version to the target
set_target_properties(ImagineStudio PROPERTIES VERSION ${PROJECT_VERSION})

# Set VS debugger working directory to repository root for consistent F5 runs
set_target_properties(ImagineStudio PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

# Propagate IMAGINE_START_MAXIMIZED option as a compile definition
if(IMAGINE_START_MAXIMIZED)
    target_compile_definitions(ImagineStudio PRIVATE IMAGINE_START_MAXIMIZED=1)
else()
    target_compile_definitions(ImagineStudio PRIVATE IMAGINE_START_MAXIMIZED=0)
endif()

# Define Unicode macros to match Visual Studio project settings
target_compile_definitions(ImagineStudio PRIVATE UNICODE _UNICODE)

# Set output directories
set_target_properties(ImagineStudio PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# stb_image library (Image Loading)
add_library(stb_image STATIC
    external/stb/stb_image_impl.cpp
)
target_include_directories(stb_image PUBLIC external/stb)

# ImGui library (Dear ImGui - Editor UI Framework)
add_library(ImGui STATIC
    external/imgui/imgui.cpp
    external/imgui/imgui_demo.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/backends/imgui_impl_dx12.cpp
    external/imgui/backends/imgui_impl_win32.cpp
)
target_include_directories(ImGui PUBLIC 
    external/imgui 
    external/imgui/backends
)

if (MSVC)
    target_link_libraries(ImagineStudio PRIVATE Msimg32)
    # Link DirectX libraries for optional DX12 backend
    target_link_libraries(ImagineStudio PRIVATE d3d12 dxgi d3dcompiler)
    # Link ImGui library
    target_link_libraries(ImagineStudio PRIVATE ImGui)
    # Link stb_image library
    target_link_libraries(ImagineStudio PRIVATE stb_image)
else()
    # On other platforms link adjustments can be added here if needed
    target_link_libraries(ImagineStudio PRIVATE ImGui)
    target_link_libraries(ImagineStudio PRIVATE stb_image)
endif()

# Benchmarks - add specific bench sources to avoid duplicate mains
set(BENCH_FILE "${CMAKE_SOURCE_DIR}/src/tests/threadpool_bench.cpp")
if (EXISTS ${BENCH_FILE})
    add_executable(threadpool_bench ${BENCH_FILE})
    target_include_directories(threadpool_bench PRIVATE ${CMAKE_SOURCE_DIR}/src)
    # Ensure ThreadPool and related job sources are linked into the bench
    file(GLOB_RECURSE JOBS_SRC "${CMAKE_SOURCE_DIR}/src/jobs/*.cpp")
    target_sources(threadpool_bench PRIVATE ${JOBS_SRC})
    target_compile_definitions(threadpool_bench PRIVATE BUILD_TEST_EXE)
    if (MSVC)
        target_link_libraries(threadpool_bench PRIVATE Msimg32)
    endif()
endif()

# Tests - add specific test sources
set(TEST_FILE "${CMAKE_SOURCE_DIR}/src/tests/taskgraph_test.cpp")
if (EXISTS ${TEST_FILE})
    add_executable(taskgraph_test ${TEST_FILE})
    target_include_directories(taskgraph_test PRIVATE ${CMAKE_SOURCE_DIR}/src)
    target_sources(taskgraph_test PRIVATE ${JOBS_SRC})
    target_compile_definitions(taskgraph_test PRIVATE BUILD_TEST_EXE)
    if (MSVC)
        target_link_libraries(taskgraph_test PRIVATE Msimg32)
    endif()
endif()

set(MAT_TEST_FILE "${CMAKE_SOURCE_DIR}/src/tests/material_test.cpp")
if (EXISTS ${MAT_TEST_FILE})
    add_executable(material_test ${MAT_TEST_FILE})
    target_include_directories(material_test PRIVATE ${CMAKE_SOURCE_DIR}/src)
    file(GLOB_RECURSE RENDER_SRC "${CMAKE_SOURCE_DIR}/src/renderer/*.cpp")
    target_sources(material_test PRIVATE ${RENDER_SRC})
    target_compile_definitions(material_test PRIVATE BUILD_TEST_EXE)
    if (MSVC)
        target_link_libraries(material_test PRIVATE Msimg32 d3d12 dxgi d3dcompiler)
        # Link ImGui library (renderer files may include imgui.h)
        target_link_libraries(material_test PRIVATE ImGui)
    endif()
endif()

# Entity System Tests
set(ENTITY_TEST_FILE "${CMAKE_SOURCE_DIR}/tests/entity_test.cpp")
if (EXISTS ${ENTITY_TEST_FILE})
    add_executable(entity_test ${ENTITY_TEST_FILE})
    target_include_directories(entity_test PRIVATE ${CMAKE_SOURCE_DIR}/src)
    # Include Scene sources (Entity, EntityManager)
    file(GLOB_RECURSE SCENE_SRC "${CMAKE_SOURCE_DIR}/src/scene/*.cpp")
    target_sources(entity_test PRIVATE ${SCENE_SRC})
    target_compile_definitions(entity_test PRIVATE BUILD_TEST_EXE)
    if (MSVC)
        target_link_libraries(entity_test PRIVATE Msimg32)
    endif()
endif()

# Transform Component Tests
set(TRANSFORM_TEST_FILE "${CMAKE_SOURCE_DIR}/tests/transform_test.cpp")
if (EXISTS ${TRANSFORM_TEST_FILE})
    add_executable(transform_test ${TRANSFORM_TEST_FILE})
    target_include_directories(transform_test PRIVATE ${CMAKE_SOURCE_DIR}/src)
    # Include Scene sources (Transform, Component)
    file(GLOB_RECURSE SCENE_SRC "${CMAKE_SOURCE_DIR}/src/scene/*.cpp")
    target_sources(transform_test PRIVATE ${SCENE_SRC})
    target_compile_definitions(transform_test PRIVATE BUILD_TEST_EXE)
    if (MSVC)
        target_link_libraries(transform_test PRIVATE Msimg32)
    endif()
endif()

# Scene Tests
set(SCENE_TEST_FILE "${CMAKE_SOURCE_DIR}/tests/scene_test.cpp")
if (EXISTS ${SCENE_TEST_FILE})
    add_executable(scene_test ${SCENE_TEST_FILE})
    target_include_directories(scene_test PRIVATE ${CMAKE_SOURCE_DIR}/src)
    # Include Scene sources (Scene, Entity, EntityManager, Transform, Component)
    file(GLOB_RECURSE SCENE_SRC "${CMAKE_SOURCE_DIR}/src/scene/*.cpp")
    target_sources(scene_test PRIVATE ${SCENE_SRC})
    target_compile_definitions(scene_test PRIVATE BUILD_TEST_EXE)
    if (MSVC)
        target_link_libraries(scene_test PRIVATE Msimg32)
    endif()
endif()

# AssetDatabase Tests
set(ASSET_DB_TEST_FILE "${CMAKE_SOURCE_DIR}/tests/asset_database_test.cpp")
if (EXISTS ${ASSET_DB_TEST_FILE})
    add_executable(asset_database_test ${ASSET_DB_TEST_FILE})
    target_include_directories(asset_database_test PRIVATE ${CMAKE_SOURCE_DIR}/src)
    # Include only AssetDatabase source (not VFS or AssetManager)
    target_sources(asset_database_test PRIVATE "${CMAKE_SOURCE_DIR}/src/assets/AssetDatabase.cpp")
    target_compile_definitions(asset_database_test PRIVATE BUILD_TEST_EXE)
    if (MSVC)
        target_link_libraries(asset_database_test PRIVATE Msimg32)
    endif()
endif()

# TextureImporter Tests
set(TEXTURE_IMP_TEST_FILE "${CMAKE_SOURCE_DIR}/tests/texture_importer_test.cpp")
if (EXISTS ${TEXTURE_IMP_TEST_FILE})
    add_executable(texture_importer_test ${TEXTURE_IMP_TEST_FILE})
    target_include_directories(texture_importer_test PRIVATE ${CMAKE_SOURCE_DIR}/src)
    # Include TextureImporter source
    target_sources(texture_importer_test PRIVATE "${CMAKE_SOURCE_DIR}/src/assets/TextureImporter.cpp")
    target_compile_definitions(texture_importer_test PRIVATE BUILD_TEST_EXE)
    if (MSVC)
        target_link_libraries(texture_importer_test PRIVATE Msimg32)
        # Link stb_image library
        target_link_libraries(texture_importer_test PRIVATE stb_image)
    endif()
endif()
