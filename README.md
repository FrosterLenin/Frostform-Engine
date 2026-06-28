# Frostform Engine

Frostform Engine is a C++ game engine built with Raylib, focused on learning engine architecture through playable demos and a custom software rasterizer path.

## Future Plans

- Camera logic with orthographic and perspective projection support.
- Additional rendering backends (OpenGL, DirectX, Vulkan).
- Expanded 3D rendering capabilities.

## Features

- **Core Engine Components**:
  - Core gameplay framework with Game, Scene, GameObject, and manager systems
  - Collision handling with colliders and collision layers/masks
  - Background support for color mode and picture mode
  - Utility math types and interpolation helper
  - Vectors math utilities
  - Custom software rasterizer module with color buffer + depth buffer

### Demos Included
![alt text](https://i.imgur.com/uU7AlPG.png)

- **Sample Games**:
  - **Pong**: Classic paddle ball game with scoring
  ![alt text](https://i.imgur.com/phetzq7.png)

  - **Space Invaders**: Retro space shooter with invaders and bullets
  ![alt text](https://i.imgur.com/TJ2aSNw.png)

  - **Rasterizer Demo** : triangle rendering modes, culling, two-sided mode
  ![alt text](https://i.imgur.com/SQXBuB3.png)

## Engine Logic: Scene, Screen, Camera, Rasterizer

This engine uses these concepts with different responsibilities:

- Scene:
  - High-level gameplay state container.
  - Owns scene-specific objects and rules (Init, Update, Draw, Destroy).
  - Examples: Pong match scene, game over scene, space invaders gameplay scene.

- Screen:
  - Low-level software framebuffer (color + depth buffers).
  - Receives pixels from the rasterizer and blits to a texture for display.
  - Used by rasterizer flow, not by every game flow.

- Camera:
  - Defines where you look from and how geometry is projected.
  - Produces view/projection behavior that affects how world-space objects are seen.
  - Camera integration is planned/ongoing for the rasterizer pipeline.

- Rasterizer:
  - Converts projected primitives into pixels/fragments.
  - Runs depth testing and shading logic, then writes into Screen.

![alt text](https://i.imgur.com/N3lS9uW.png)

Conceptual pipeline:

1. Scene updates game objects.
2. Camera defines view/projection for visible geometry.
3. Rasterizer converts geometry into fragments.
4. Screen stores color/depth results.
5. Final image is presented in the window.

Planned render-backend selection:

- The engine will expose a render strategy/backend selection layer to decide which rendering path is used at runtime or startup.
- Initial targets are:
  - Software rasterizer path (custom `Screen` + `Rasterizer`).
  - Raylib immediate-mode drawing path.
  - Future backends such as OpenGL, DirectX, and Vulkan.
- Goal: keep gameplay and scene logic backend-agnostic while swapping only the rendering implementation.

## Why Only RasterizerDemo Uses Screen

Pong and Space Invaders currently render with direct Raylib draw calls (rectangles, circles, textures), so they do not need the software framebuffer. 
RasterizerDemo uses the custom rasterizer, so it needs Screen as its render target.

## Prerequisites

### Required Tools

- **CMake** (version 3.16 or higher)
  - Download: https://cmake.org/download/
  - Installation: Follow the installer for your platform

- **C++ Compiler**
  - **MSVC** (Windows): Install Visual Studio Community (free)
    - Download: https://visualstudio.microsoft.com/downloads/
    - Select "Desktop development with C++" workload during installation
  - **GCC** (Cross-platform): 
    - Windows: Install MinGW-w64 from https://www.mingw-w64.org/
    - Linux/macOS: Usually pre-installed or available via package manager
  - **Clang** (Cross-platform):
    - Windows: Available through Visual Studio or LLVM installer
    - Download: https://llvm.org/

- **Raylib Library** (automatically downloaded and built via CMake FetchContent)
  - No manual installation required - handled by the build process

## Building
1. Clone or download the project

### Option 1a: Manual Build
2. Navigate to the project root directory
3. Create a build directory:
   ```
   mkdir build  # create build directory
   cd build     # enter build directory
   ```
4. Generate build files with CMake:
   ```
   cmake ..     # execute into the ./build directory
   ```
5. Build the project:
   ```
   cmake --build . --config Release   # execute into the ./build directory - builds the code into the .exe into release folder
   ```

### Option 1b: Manual Build
2. Open the project
3. Generate build files with CMake::
   ```
   cmake -S . -B build    # CMAKE initial build
   ```
4. Build the project:
   ```
   cmake --build build    # builds the code into the .exe
   ```

### Option 2: Using Build Script (Windows)
2. Run the PowerShell build script from the project root:
```
.\scripts\build.ps1 -BuildType Release
```
0. To clean and rebuild:
```
.\scripts\build.ps1 -Clean -BuildType Release
```

## Running

After building, the executable will be located in `build/bin/Release/`. Run the game engine to start with the default game samples, or modify the code to select different games.

Launcher controls:

- Press P for Pong.
- Press S for Space Invaders.
- Press R for Rasterizer Demo.

## Project Structure

- `include/` - Header files for engine components and games
- `src/` - Source files for implementation
- `build/` - Build artifacts (generated by CMake)
- `scripts/` - Build and utility scripts
- `CMakeLists.txt` - CMake build configuration
- `README.md` - Project documentation
- `CHANGELOG.md` - Version history and changes
- `CONTRIBUTING.md` - Guidelines for contributors
- `LICENSE` - MIT license file
- `.clang-format` - Code formatting configuration
- `.editorconfig` - Editor configuration
- `VERSION` - Current version number

## Contributing

Feel free to contribute by adding new games, improving the engine, or fixing bugs. Please ensure code follows C++ best practices and includes appropriate documentation.

## Greetings

I would like to express my sincere gratitude to the following:

- **Raylib** – for providing a simple and powerful library for videogame programming, and to its community and developers for their continuous work and support.
  - Website: https://www.raylib.com/
  - License: zlib/libpng

- **AIV (Accademia Italiana Videogiochi)** – and its teachers for their guidance, support, and for helping me build the knowledge and foundation to implement this code.
  - Website: https://www.aiv01.it/

## License

This project is licensed under the MIT License - see the LICENSE file for details.