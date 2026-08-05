# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]
### TODO
- ~~Implement Camera logic with Orthographic and Perspective projections~~ ✓ Done
- ~~Integrate camera world-to-screen projection into Pong and Space Invaders~~ ✓ Done

### Future Plans
- OpenGL, DirectX, and Vulkan backends
- 3D rendering capabilities: Raytracing

### Known Issues 
#### Contributions or suggestions to help identify and fix these issues are welcome.
- Space Invaders: The enemy formation gradually shifts, causing inconsistent spacing between invaders over time after the enemy manager update.

### Added
#### 2026-06-17
- **Pong in-game timer UI component**
  - Added `Timer` UI integration to `PongGameScene::InitUI()`
  - Timer is spawned and bound through `UIManager` so it updates every frame
  - Timer is rendered at the top-center in `MM:SS` format during gameplay

- **Camera system integration for Pong and Space Invaders**
  - Scene camera setup: `PongGameScene` and `SpaceInvadersGameScene` now create `OrthographicCamera` instances in `Init()`
  - World-to-screen projection helpers in camera classes: `ProjectTopLeft2D()`, `GetProjectedDepth()`, `ProjectRectangleTopLeft()` on `ACamera` with implementations in `PerspectiveCamera` and `OrthographicCamera`
  - GameObject camera ownership: added public `SetCamera()` / `GetCamera()` to GameObject base class with automatic scene camera fallback
  - Renderable objects in Pong/Space Invaders now use projected coordinates: `Paddle`, `Circle` (Ball/Bullet), `Invader`, `SpaceShip` all apply camera projection in `Draw()`
  - Scene manager exposure: added `Game::GetSceneManager()` for cross-subsystem access to active scene

#### 2026-05-26
- **Pong software-raster mode integration (NONE/BBOX)**
  - Added `RasterMode::NONE` to preserve default raylib rendering when software rasterization is disabled
  - Added raster-mode state and shared software `Screen` management to `Game` (`SetRasterMode`, `GetRasterMode`, `GetScreen`)
  - Added `Rasterizer::DrawCircle(IVector2 center, int radius, Color color, Screen* screen)` using bounding-box fill + radius test
  - Added pong-level raster-mode configuration through `PongGame(FVector2, RasterMode)` with default `RasterMode::NONE`
  - Added `DrawManager::DrawLayers(startLayer, endLayer)` to support precise per-layer ordering when mixing software raster + raylib UI

#### 2026-05-22
- **Camera system for the software rasterizer pipeline**
  - `ACamera` abstract base class (`include/core/rasterizer/ACamera.hpp`) with `Project()`, `WorldToCameraSpace()`, `GetPosition()`, `SetPosition()`, `IsFaceCulled()`
  - `PerspectiveCamera` implementation with configurable FOV and position (`PerspectiveCamera.hpp/.cpp`)
  - `OrthographicCamera` implementation with configurable orthoSize and aspect ratio (`OrthographicCamera.hpp/.cpp`)
  - Camera integrated into `Scene` base class as optional `std::unique_ptr<ACamera> _Camera` with `GetCamera()` accessor
  - Real-time camera movement via keyboard: W/S (Y), A/D (X), Q/E (Z), Numpad +/- (FOV)
  - Camera info HUD in top-right corner showing position (X, Y, Z) and current FOV
  - New `InputAction` enum values: `CAM_UP`, `CAM_DOWN`, `CAM_LEFT`, `CAM_RIGHT`, `CAM_FORWARD`, `CAM_BACK`, `CAM_FOV_UP`, `CAM_FOV_DOWN`

### Changed
#### 2026-08-02
- **Input and gameplay consistency pass**
  - `InputManager` now samples mouse position directly from raylib every frame and updates mouse wheel delta in `Update()`
  - Restored mouse button semantics to preserve held-button behavior: `GetMouseButtonDown()` maps to `IsMouseButtonDown()`, while `GetMouseState()` maps to one-shot `IsMouseButtonPressed()`
  - `Bullet::Start()` no longer overrides `_AccelerationIndex`, so constructor-provided bullet speed values are preserved
  - Invader shot cadence state moved to header-side inline static initialization (`Invader::_TimeSinceLastShot`) and kept explicit scene reset on `SpaceInvadersGameScene::Init()`

- **Rasterizer texture ownership safety**
  - `TextureCpu::LoadFromFile()` now returns `std::unique_ptr<TextureCpu>`
  - `Gpu::Texture` now owns texture memory via `std::unique_ptr<TextureCpu>`
  - Rasterizer texture sampling path updated to use `gpu.Texture.get()` after null checks

- **Maintenance cleanup and defensive guards**
  - Removed unused `EnemyManager::SpawnEnemy(...)` declaration from the public API
  - Added `scoreY` fallback handling/comment in `PongGameScene::InitUI()` to keep score text readable if timer spawn/lock is unavailable
  - Corrected non-portable standard include case in `Collider.hpp` (`<Algorithm>` -> `<algorithm>`)
  - Removed redundant unsigned bounds check branch in `DrawManager::BindObject()`
  - Replaced multiple `weak_ptr.lock().get()` assignments in scene initialization code with explicit `shared_ptr` lock checks before storing raw pointers
  - Guarded `EnemyManager::Bind(...)` calls in Space Invaders enemy spawn setup to avoid binding null entries when a lock fails
  - Renamed collision helper typo `CheckForCollissionPair` -> `CheckForCollisionPair` in `CollisionManager`
  - Renamed math constant `PI2` to `PiValue` to match its actual value usage in degree-to-radian conversion
  - Updated core CMake globs to use `CONFIGURE_DEPENDS` only for core modules (`src/core`, `src/core/managers`, `src/core/rasterizer`)
  - Removed unused `src/core/enums/*.cpp` source glob from CMake (enums are header-only)
  - Made `Ball` speed-cap logic opt-in with explicit `_HasSpeedCap` state to avoid relying on an unset cap
  - `Circle::SetRadius()` now keeps `_Size` aligned to diameter (`radius * 2`) after runtime radius changes
  - Enforced horizontal-only player ship movement in Space Invaders by ignoring vertical input in `SpaceShip::UpdateWithInput()`
  - `Ball::Update()` now returns immediately after score reset so the ball does not move twice in the same scoring frame
  - Added explicit `[[maybe_unused]]` documentation comment in `Ball::Update()` for debug-ready local input access
  - `UIManager` event bindings now use `std::weak_ptr<GameObject>` keys instead of raw pointers to avoid stale pointer lifetime hazards
  - Added lazy cleanup of expired `UIManager` event bindings during trigger iteration
  - Added constructor/setter clamping guards in `OrthographicCamera` to prevent invalid projection parameters (zero/negative size or screen dimensions)
  - Updated `PerspectiveCamera` projection handling for behind-camera points (discarded outside viewport) and clamped minimum FOV
  - Added explicit CMake comment documenting that `CONFIGURE_DEPENDS` auto-reload is intentionally enabled only for core engine modules
  - Expanded legacy documentation comment for `Circle::CheckCollision()` to clarify it is a historical fallback path

#### 2026-06-17
- **CMake source glob refresh for core files**
  - Updated `SRC_FILES_CORE` to `file(GLOB ... CONFIGURE_DEPENDS ...)` so new files like `Timer.cpp` are detected without stale generated project state

- **Projection logic moved to camera classes** — Camera classes now own all projection math including 2D-to-centered-world conversion and depth calculation; `GameObject` helpers delegate to camera methods rather than duplicating logic
- `Invader::Draw()` and `SpaceShip::Draw()` now apply projected rectangle when scene camera is present, falling back to screen-space drawing
- `Circle::Draw()` now applies projected center coordinates when scene camera is present
- `Paddle::Draw()` now applies projected rectangle in both raylib and software raster paths
- Removed `GetSceneCamera()` from GameObject in favor of single unified `GetCamera()` that resolves object override first, then scene camera as fallback

#### 2026-05-26
- `Paddle::Draw()` now supports BBOX raster rendering by decomposing the paddle rectangle into two triangles
- `Circle::Draw()` now branches by raster mode: raylib path for `NONE`, software raster circle path for `BBOX_TRIANGLE`
- `PongGameScene::Draw()` now integrates software-screen clear/blit flow for rasterized gameplay objects
- `main.cpp` Pong launcher now uses a configurable raster mode constant for quick mode testing
- **Pong UI draw-order regression in raster mode**
  - `ScoreUI` visibility restored by ensuring UI is rendered after software framebuffer blit
  
#### 2026-05-22
- `RasterizerDemoScene` creates a `PerspectiveCamera` (fov=1.5, pos={0,0,-4}) matching previous hardcoded projection — no visual change
- `RasterizerTriangleObject` now uses camera's `Project()` instead of its own local projection method
- Removed `RasterizerTriangleObject::Project()` private method (projection delegated to camera)

### Fixed
#### 2026-08-05
- **Core lifecycle and construction safety pass**
  - Fixed `GameObject` default constructor arguments in `GameObject.hpp` to valid brace-initialized `FVector2` values, removing the comma-operator default argument pitfall
  - Added safe default initialization for `GameObject::_Color` in the base constructor to avoid latent undefined state when color-specific constructors are not used
  - Fixed `GameObject::SetPosition()` clamping for oversized objects by guarding max bounds with `std::max(0, ...)`, preventing invalid clamp ranges
  - Made `Game` destructor virtual and updated `Game::~Game()` teardown order to release engine objects and texture-owning resources before `CloseWindow()`
  - Hardened `Ball::OnCollisionEnter()` bounce-axis resolution for ambiguous corner contacts and added deterministic tie-breaking using incoming velocity
  - Enforced strict `Ball` speed-cap behavior by clamping post-acceleration value to the configured cap
  - Synchronized `Circle::SetRadius()` with `CircleCollider` radius to keep runtime visual size and collision radius aligned

#### 2026-08-02
- **Legacy and fallback behavior fixes**
  - Fixed legacy `Paddle::CheckCollision()` chained-comparison logic by replacing invalid chained comparisons with explicit AABB overlap checks
  - Preserved bullet owner-callback behavior by using callback-first expiration handling with explicit local deactivation fallback when no callback is bound
  - Split bullet boundary handling into dedicated vertical and horizontal checks; horizontal expiry is currently disabled by design for vertical-only projectiles

#### 2026-06-17
- **ScoreUI horizontal offset alignment**
  - Corrected score text horizontal offset handling in `ScoreUI::Draw()` to keep player score placement consistent across layouts

## [0.15.0] - 2026-05-18
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
- **Rasterizer backface culling and two-sided rendering**
  - `Rasterizer::SetFaceCullingEnabled(bool)` and `IsFaceCullingEnabled()` to control backface culling
  - `Rasterizer::SetTwoSidedRenderingEnabled(bool)` and `IsTwoSidedRenderingEnabled()` for two-sided rendering mode
  - Private helper `Rasterizer::IsFrontFacing()` uses cross product to determine triangle facing direction
  - Mode 4 added to rasterizer demo: **Scanline + Phong (2-Sided)** - renders triangle with both faces colored and lit
  - Input action `RASTER_MODE_4` bound to `KEY_FOUR` in rasterizer demo scenes
  - Back face lighting fixed by flipping normals when rendering backfacing triangles

### Changed
#### 2026-05-14
- **StatBar refactoring and Space Invaders life UI**
  - Added life bar UI to Space Invaders using `StatBar` directly
  - Made `_MaxPoints` and `_CurrentPoints` protected for potential subclass access
  - Simplified `StatBarEventCallback` to single-parameter signature: `std::function<void(int)>` (current value only)
  - Life bar displayed at bottom-left corner (10px, screen height - 30px) with green fill, updates on damage
  - `UIManager` template instantiations updated for `StatBar` with single-parameter events

### Fixed
#### 2026-05-14
- **SceneManager initialization order bug**
  - Fixed `SceneManager::LoadScene()` to set `_CurrentScene` before calling `Init()`

#### 2026-05-12
- **Refactored scene management:** Removed all explicit scene data members (e.g., _GameScene, _GameOverScene) from game implementations (`PongGame`, `SpaceInvaders`, `RasterizerDemo`). Scene access and management are now handled exclusively via `SceneManager`. All direct assignments like `SetGameScene(this)` in scene classes have been eliminated. This enforces a single source of truth for the current scene and improves maintainability.

#### 2026-05-09
- **Pong scene transition crash fix**
  - `PongGameOverScene` now receives final scores by value instead of storing dangling scene pointer
  - Scene transition set at game-end time in `PongGameScene::Update()` to avoid use-after-free
  - Removed outdated pre-computed next-scene setup from `PongGame::InitGame()`
- **Rasterizer mode 3 (Phong) backface rendering**
  - Added backface culling to prevent rendering of triangles facing away from camera (improves performance)
  - Mode 3: Backface culling enabled (default behavior)

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