cmake_minimum_required(VERSION 3.15)
project(ImagineStudio LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Collect sources
file(GLOB_RECURSE SRC_FILES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.cxx"
    "${CMAKE_SOURCE_DIR}/src/*.cc"
)

add_executable(ImagineStudio ${SRC_FILES})

target_include_directories(ImagineStudio PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Define Unicode macros to match Visual Studio project settings
target_compile_definitions(ImagineStudio PRIVATE UNICODE _UNICODE)

# Set output directories
set_target_properties(ImagineStudio PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
