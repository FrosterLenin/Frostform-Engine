# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased] - 2026-04-30

### Added
- `StatBar` UI component for displaying scaled bars (life, mana, etc.) with configurable max/current, fill/background/border colors, and optional text overlay
- `StatBar` events: `OnValueChanged(int current, int max)`, `OnDepleted()`, and `OnFilled()` for reactive HUD and gameplay systems
- `StatBar` mutators: `AddPoints()`, `RemovePoints()`, `SetCurrent()`, `SetMax(value, refill)`, `Refill()` with automatic clamping and event firing
- `StatBar` accessors: `GetCurrent()`, `GetMax()`, `GetRatio()`, `IsDepleted()`, `IsFull()` for queries
- `Invader::SHOOT_INTERVAL` (2s) and shared static `_TimeSinceLastShot` timer so all invaders share a single shooting cadence
- `TickInvaderShot(float deltaTime)` private method in SpaceInvaders centralising invader shooting logic
- Random-start forward-walk shooter selection: each interval a random active invader is chosen, guaranteeing a shot as long as at least one invader is alive
- `EnemyManager::GetManagedObjects()` getter exposing the managed object list for external iteration
- `CollisionLayer` assignments for SpaceShip (`PLAYER` layer, `ENEMY | PROJECTILE_2` mask) and Invader (`ENEMY` layer, `PLAYER | PROJECTILE` mask) with matching comments
- `FCollisionInfo::OtherObject` so collision callbacks can identify which object was hit
- `Bullet::DAMAGE` constant for projectile-specific damage handling

### Changed
- Invader bullet pool spawned with `isPlayer = false`, giving them `PROJECTILE_2` layer so they correctly collide with the player but not with other enemy bullets
- `Game::InitGame` loop now captures the initial `_GameObjects` count before iterating, preventing iterator invalidation when `Start()` spawns pooled bullets during init
- Shooter selection moved from per-invader `Update` to a single centralised call in `SpaceInvaders::Update` via `TickInvaderShot`
- Invader shooting loop iterates `EnemyManager::GetManagedObjects()` instead of the full `_GameObjects` list
- Collision callbacks now receive side-specific collision info, including the opposing GameObject pointer for each participant
- `SpaceShip::OnCollisionEnter` now applies type-specific damage for enemy invaders vs enemy bullets (`PROJECTILE_2`)

### Fixed
- Program freeze at `RegisterCollider` during `InitGame` caused by vector reallocation when bullet pools were created inside `Start()` while the init loop was still iterating `_GameObjects`

## [Unreleased] - 2026-04-29

### Added
- SpaceShip life points system: starts with 100 life and deactivates when it reaches 0
- `OnCollisionEnter` override on SpaceShip to apply damage on contact
- `GetLife()` and `TakeDamage()` accessors on SpaceShip
- `Invader::CollisionDamage` (10) constant defining damage dealt to the SpaceShip on collision
- `CollisionLayer` bitflag enum (`include/core/enums/CollisionLayers.hpp`) with bitwise operators (`|`, `&`, `~`, `|=`, `&=`) and `HasLayer()` helper to express collision categories
- `_Layer` and `_Mask` members on `Collider` plus new constructors taking `(layer, mask)` for `Collider`, `RectangleCollider`, and `CircleCollider`
- `Collider::CanCollide(A, B)` static helper performing symmetric layer/mask checks

### Changed
- Moved `_Owner` (parent GameObject pointer) from Bullet to base GameObject class so any GameObject can optionally have an owner/parent
- Added `SetOwner()` and `GetOwner()` methods to GameObject public API
- `_Owner` initialized to `nullptr` by default in GameObject constructor
- Bullet now uses inherited `_Owner` instead of its own private member
- SpaceShip bullet pool increased from 2 to 3 bullets with reduced radius (5.f)
- `CollisionManager::Update` now skips pairs where objects are the same instance, in an owner/child relationship, or whose `Collider` layer/mask configuration disallows the collision

### Future Plans
- OpenGL, DirectX, and Vulkan backends
- 3D rendering capabilities

### Known Issues 
#### Contributions or suggestions to help identify and fix these issues are welcome.
- Space Invaders: The enemy formation gradually shifts, causing inconsistent spacing between invaders over time after the enemy manager update.

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