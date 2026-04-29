# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased] - 2026-04-29

### Changed
- Moved `_Owner` (parent GameObject pointer) from Bullet to base GameObject class so any GameObject can optionally have an owner/parent
- Added `SetOwner()` and `GetOwner()` methods to GameObject public API
- `_Owner` initialized to `nullptr` by default in GameObject constructor
- Bullet now uses inherited `_Owner` instead of its own private member
- SpaceShip bullet pool increased from 2 to 3 bullets with reduced radius (5.f)

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