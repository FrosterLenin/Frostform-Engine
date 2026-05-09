# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]
### TODO
- Game implementations should not have explicit scene data members
- Implement Camera logic with Orthographic and Perspective projections

### Future Plans
- OpenGL, DirectX, and Vulkan backends
- 3D rendering capabilities

### Known Issues 
#### Contributions or suggestions to help identify and fix these issues are welcome.
- Space Invaders: The enemy formation gradually shifts, causing inconsistent spacing between invaders over time after the enemy manager update.

### Added
#### 2026-05-09
- **Background gameObject cleanup and dual-mode implementation**
  - `BackgroundMode` enum (COLOR, PICTURE) moved to `core/enums/BackgroundMode.hpp`
  - `Background` COLOR mode: drives `Game::SetClearColor()` during updates so background controls clear color
  - `Background` PICTURE mode: loads image via `LoadImage()` + `LoadTextureFromImage()`, renders fullscreen with `DrawTexturePro()`
  - Picture-mode image loading with robust relative path resolution (parent directory walk from current working directory)
  - Proper texture lifecycle: loaded in `Start()`, unloaded on mode switch or destruction
  - Scene-level API: `SetColorMode(Color)` and `SetPictureMode(std::string path, Color clearColor)`
  - Applied picture-mode background spawning to `SpaceInvadersGameScene`, `PongGameScene`, and `RasterizerDemoScene`
  - Picture assets loaded from `src/spaceInvaders/resources/space.jpg` fallback to black clear color on load failure

### Fixed
#### 2026-05-09
- **Pong scene transition crash fix**
  - `PongGameOverScene` now receives final scores by value instead of storing dangling scene pointer
  - Scene transition set at game-end time in `PongGameScene::Update()` to avoid use-after-free
  - Removed outdated pre-computed next-scene setup from `PongGame::InitGame()`

### Changed
#### 2026-05-07
- **Rasterizer demo refactor** to unified Game/Scene architecture
  - `RasterizerDemo` now inherits from `Game` (matching Pong/SpaceInvaders pattern)
  - `RasterizerDemoScene` manages scene lifecycle with manager-driven updates
  - `RasterizerTriangleObject` GameObject handles triangle rotation and rasterization rendering
  - `RasterizerModeUI` UIObject handles HUD text and mode display
  - Input actions `RASTER_MODE_1`, `RASTER_MODE_2`, `RASTER_MODE_3` added to `InputActions` enum
  - Scene-driven rendering through `DrawManager` and UI updates through `UIManager` event binding
  - All rasterizer initialization and update logic now flows through core Game managers

#### 2026-05-07 (RasterizerDemo refactor changes)
- **RasterizerDemo** refactored from standalone loop to Game subclass
  - Constructor now takes `FVector2 screenSize` parameter (default `{800, 450}`)
  - `InitGame(clearColor)` override initializes scene manager with `RasterizerDemoScene`
  - Rendering and input handling delegated to scene and spawned game objects
  - Removed direct window ownership and manual loop; inherits `Game::Run()` behavior
  - Consolidate math utils, clean up GameObject base methods, unify game initialization

#### 2026-05-06
- **Software rasterizer module** under `core/rasterizer/`
  - `Rasterizer` class unifying line and triangle rasterization with both explicit methods (`DrawLine`, `DrawTriangle`) and a `Draw(RasterMode, ...)` mode-driven dispatcher
  - `RasterMode` enum (DDA_LINE, BBOX_TRIANGLE, SCANLINE_TRIANGLE) in `core/enums/`
  - `GpuDrawMode` enum (NONE, COLOR, TEXTURE) in `core/enums/`
  - `Gpu` and `GpuVertex` structs for the scanline path with Phong lighting (ambient + Lambert diffuse + specular)
  - `Screen` class providing a software framebuffer with color buffer, depth buffer, and raylib `Texture2D` blit
  - `TextureCpu` class for CPU-side RGBA texture loading via raylib `LoadImage`
- **Rasterizer demo** (`RasterizerDemo`)
  - Standalone runner with rotating triangle rendered in three switchable modes (`1` wireframe, `2` filled bbox, `3` scanline + Phong)
  - `[R] Rasterizer Demo` entry added to the main launcher menu
- **Math utility types** under `utility/`
  - `IVector2` integer 2D vector for screen-space coordinates
  - `FVector3` 3D float vector with `RotateY`, `Normalize`, `Magnitude`, `Dot`, `Reflect`, `Cross`, and arithmetic operators
  - `FXColor` RGBA color with saturating scalar multiply / add operators for lighting math
  - `FMaths` namespace with `Min3`, `Max3`, `Det` (edge function), and `PI2`
  
#### 2026-05-06
- `CMakeLists.txt` now globs `src/core/rasterizer/*.cpp` into the build
- Renamed FVector.cpp to FVector2.cpp

## [0.10.0] - 2026-05-05

### Added
- **Scene-based game architecture**
  - `Scene` base class for managing game states and object lifecycles
  - `SceneManager` for orchestrating scene transitions and lifecycle
  - `SceneResult` enum for scene state communication (CONTINUE, EXIT, RESTART, SCENE_COMPLETE)
  - `SceneState` enum for tracking scene loading/active/unloading states
  - Scene template methods `SpawnGameObject<T>()` and `SetNextScene<T>()` for flexible scene setup
  - Scene-specific game object management separate from Game's global object list
- **PongGame implementation** using scene architecture
  - `PongGameScene` for active gameplay with paddles and ball
  - `PongGameOverScene` for game completion screen
- **SpaceInvaders implementation** using scene architecture
  - `SpaceInvadersGameScene` for active gameplay with enemies and player ship
- `AddPlayer()` method to ScoreUI for explicit player registration from game scenes

### Changed
- All game object spawning now handled through `Scene::SpawnGameObject()` for scene-local object management
- Input management now scene-based via `InputManager` bound during scene initialization
- Making `UIObject` set correct `DrawLayer` (UI) in constructor
- Activating `ScoreUI` in `Start()` method so `DrawManager` renders it
- Initializing `DrawManager` layer vector immediately in constructor
- Preventing `ScoreUI` from clearing players in `UpdateControlled()`
- Having game scenes explicitly register players via `AddPlayer()`
- `DrawManager` layer availability by initializing in constructor instead of deferred initialization

## [0.6.0] - 2026-05-01

### Added
#### 2026-04-30
- `UIObject<Func>` template base class for UI elements with configurable `UpdateEvent` property
- `StatBar` UI component for displaying scaled bars with configurable max/current values and visual styling
- `FCollisionInfo::OtherObject` to identify the opposing GameObject in collision callbacks
- `Invader::SHOOT_INTERVAL` and centralized invader shooting via `TickInvaderShot()` in SpaceInvaders
- `CollisionLayer` bitflag enum with layer/mask assignments for SpaceShip and Invader
- `EnemyManager::GetManagedObjects()` getter for external iteration

#### 2026-04-29
- SpaceShip life points system: starts with 100 life and deactivates when it reaches 0
- `OnCollisionEnter` override on SpaceShip to apply damage on contact
- `GetLife()` and `TakeDamage()` accessors on SpaceShip
- `Invader::CollisionDamage` (10) constant defining damage dealt to the SpaceShip on collision

### Changed
#### 2026-04-30
- `ScoreUI` and `StatBar` now inherit from `UIObject<Func>` instead of `GameObject`
- Invader bullet pool layer set to `PROJECTILE_2` for correct collision with SpaceShip
- Collision detection uses symmetric layer/mask checking via `Collider::CanCollide()`
- Moved `_Owner` (parent GameObject pointer) from Bullet to base GameObject class
- SpaceShip bullet pool increased from 2 to 3 bullets with reduced radius (5.f)

#### 2026-04-29
- Added `SetOwner()` and `GetOwner()` methods to GameObject public API
- `CollisionManager::Update` now skips pairs where objects are the same instance or in an owner/child relationship

### Fixed
#### 2026-04-30
- Program freeze at `RegisterCollider` during `InitGame` caused by vector reallocation when bullet pools were created inside `Start()` while the init loop was still iterating `_GameObjects`

## [0.4.0] - 2026-04-28

### Added
- Event-based architecture for UI components
- `ScoreEventCallback` type alias for score event signatures `(playerIndex, score)`
- `OnScoreUpdated` public event member in ScoreUI for external binding
- `TriggerObjectEvent<T>(Args...)` template in UIManager to trigger events by object type
- `GetManagedObject<T>()` template in UIManager to retrieve managed objects by type
- UIManager moved to base Game class (accessible via `GetUIManager()`)
- Score system integration for Space Invaders (invaders give 10 points when killed)
- `InitUI()` and `ScorePoint()` methods in SpaceInvaders

### Changed
- ScoreUI event renamed from `ScoreEvent` to `OnScoreUpdated`
- ScoreUI event signature changed from `void(int)` to `void(int, int)` for (playerIndex, score)
- `UpdateScore()` now accepts both playerIndex and points parameters
- Ball now triggers score events through UIManager instead of calling PongGame directly
- PongGame binds OnScoreUpdated event in InitUI with lambda to ScorePoint
- Removed PongGame-specific UIManager instance (now uses inherited one from Game)

### Removed
- `TriggerObjectEvent(GameObject*, Args...)` overload (replaced by type-based version)
- ScoreUI constructor event initialization (events now bound externally by games)

## [0.3.0] - 2026-04-26

### Added
- Moved all common manager methods (`Bind()`, `Init()`, `Update()`, `IsEmpty()`) to BaseManager with default implementations
- Moved ScoreUI from PONG to Core and refactor to support a configurable number of players

## [0.2.0] - 2026-04-25

### Added
- UIManager event binding system with variadic template support
- Support for events with any number of parameters (0, 1, or more)
- Template function explicit instantiation for cleaner code organization
- Generalized bound objects storage in BaseManager to reduce code duplication across managers

### Fixed
- PongGame initialization crash by creating UIManager before InitUI()
- ScoreUI event binding issue by initializing ScoreEvent in constructor
- Event triggering now properly calls bound UI object events

## [0.1.0] - 2026-04-24

### Added
- Initial game engine framework with core components
- Pong game implementation
- Space Invaders game implementation
- Raylib integration for rendering
- CMake build system