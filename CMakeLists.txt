cmake_minimum_required(VERSION 3.15)
project(ImagineStudio LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Collect sources
file(GLOB_RECURSE SRC_FILES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.cxx"
    "${CMAKE_SOURCE_DIR}/src/*.cc"
    "${CMAKE_SOURCE_DIR}/src/*/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*/*/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*/*/*/*.cpp"
)

file(GLOB_RECURSE ASSET_SRC "${CMAKE_SOURCE_DIR}/src/assets/*.cpp")
list(APPEND SRC_FILES ${ASSET_SRC})

# Ensure renderer sources are included (explicit glob)
file(GLOB_RECURSE RENDER_SRC "${CMAKE_SOURCE_DIR}/src/renderer/*.cpp")
list(APPEND SRC_FILES ${RENDER_SRC})

# Ensure assets VFS is included
list(APPEND SRC_FILES "${CMAKE_SOURCE_DIR}/src/assets/VFS.cpp")

# Ensure UI sources are included
file(GLOB_RECURSE UI_SRC "${CMAKE_SOURCE_DIR}/src/ui/*.cpp")
list(APPEND SRC_FILES ${UI_SRC})

# Explicitly ensure renderer stubs added by recent iterations are included
list(APPEND SRC_FILES "${CMAKE_SOURCE_DIR}/src/renderer/Fence.cpp")

add_executable(ImagineStudio ${SRC_FILES})

target_include_directories(ImagineStudio PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Define Unicode macros to match Visual Studio project settings
target_compile_definitions(ImagineStudio PRIVATE UNICODE _UNICODE)

# Set output directories
set_target_properties(ImagineStudio PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

if (MSVC)
    target_link_libraries(ImagineStudio PRIVATE Msimg32)
endif()
