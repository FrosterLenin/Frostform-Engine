cmake_minimum_required(VERSION 3.16)
project(FrostformEngine)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) # BIN is the out folder of the binary

include(FetchContent)
FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/5.5.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP ON
)

# BUILD OPTIONS FOR RAYLIB
set(BUILD_EXAMPLES OFF CACHE BOOL "Build Examples" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "Build Games" FORCE)
set(OPENGL_VERSION "4.3" CACHE STRING "OpenGL version" FORCE)

FetchContent_MakeAvailable(raylib)

# we use the name giving in project()
file(GLOB SRC_FILES_UTILITY "src/utility/*.cpp")
# CONFIGURE_DEPENDS is used to make sure CMake re-evaluates the glob if files are added or removed
# We intentionally enable auto-reconfigure only for engine core folders.
# Gameplay/demo modules are examples and are kept explicit to avoid extra CMake churn.
file(GLOB SRC_FILES_CORE CONFIGURE_DEPENDS "src/core/*.cpp")
file(GLOB SRC_FILES_CORE_MANAGERS CONFIGURE_DEPENDS "src/core/managers/*.cpp")
file(GLOB SRC_FILES_PONG "src/pong/*.cpp")
file(GLOB SRC_FILES_SPACE_INVADERS "src/spaceInvaders/*.cpp")
file(GLOB SRC_FILES_CORE_RASTERIZER CONFIGURE_DEPENDS "src/core/rasterizer/*.cpp")
file(GLOB SRC_FILES_RASTERIZER_DEMO "src/rasterizerDemo/*.cpp")

add_executable(${PROJECT_NAME} src/main.cpp 
    ${SRC_FILES_UTILITY} 
    ${SRC_FILES_CORE} 
    ${SRC_FILES_CORE_MANAGERS} 
    ${SRC_FILES_PONG} 
    ${SRC_FILES_SPACE_INVADERS}
    ${SRC_FILES_CORE_RASTERIZER}
    ${SRC_FILES_RASTERIZER_DEMO}
)

# link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
target_include_directories(${PROJECT_NAME} PRIVATE include)

# getting needed libraries per OS
# FOR WINDOWS : winmm and gdi32
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE winmm gdi32)
endif()
if(APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE 
        "-framework OpenGL"
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
    )
endif()