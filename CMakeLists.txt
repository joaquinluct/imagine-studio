cmake_minimum_required(VERSION 3.15)
project(ImagineStudio LANGUAGES CXX)

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
# Ensure TaskGraph is included
list(APPEND SRC_FILES "${CMAKE_SOURCE_DIR}/src/jobs/TaskGraph.cpp")

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
else()
    # On other platforms link adjustments can be added here if needed
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
