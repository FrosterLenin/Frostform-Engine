#pragma once

#include <vector>
#include <memory>
#include <stdexcept>
#include <functional>
#include <tuple>
#include "core/GameObject.hpp"
#include "core/enums/SceneResult.hpp"
#include "core/enums/SceneState.hpp"
#include "core/rasterizer/ACamera.hpp"

class Game;

// Base class for all scenes. Scenes manage GameObjects and the logic for a specific game state
// Examples: PongGameScene, PongGameOverScene, SpaceInvadersGameScene, etc...
class Scene
{
protected:
    Game* _Game;
    SceneState _State;
    std::vector<std::shared_ptr<GameObject>> _GameObjects;
    std::unique_ptr<ACamera> _Camera;
public:
    Scene(Game* game);
    virtual ~Scene();

    // Scene lifecycle
    virtual void Init() = 0;
    virtual SceneResult Update(float deltaTime) = 0;
    virtual void Draw() = 0;
    virtual void Destroy() = 0;

    // Game object management
    std::shared_ptr<GameObject> SpawnGameObject(std::shared_ptr<GameObject> gameObject);
    const std::vector<std::shared_ptr<GameObject>>& GetGameObjects() const;
    SceneState GetState() const;
    Game* GetGame() const;
    ACamera* GetCamera() const;

    template<typename T, typename... Args>
    std::weak_ptr<T> SpawnGameObject(Args&&... args) {
        if (!std::is_base_of<GameObject, T>::value) {
            throw std::invalid_argument("T must be a GameObject");
        }
        std::shared_ptr<T> gameObject = std::make_shared<T>(std::forward<Args>(args)...);
        std::shared_ptr<GameObject> result = RegisterGameObject(gameObject);
        return std::static_pointer_cast<T>(result);
    }

    // Scene transition - set the next scene to load when this scene completes
    template<typename T, typename... Args>
    void SetNextScene(Args&&... args) {
        if (!std::is_base_of<Scene, T>::value) {
            throw std::invalid_argument("T must be a Scene");
        }
        // Capture arguments by value in a tuple
        auto argsTuple = std::make_tuple(std::forward<Args>(args)...);
        _NextSceneCreator = [argsTuple](Game* game) -> std::unique_ptr<Scene> {
            return std::apply([game](auto&&... capturedArgs) {
                return std::make_unique<T>(game, std::forward<decltype(capturedArgs)>(capturedArgs)...);
            }, argsTuple);
        };
    }
    
    
    bool HasNextScene() const; // Check if this scene has a next scene configured
    std::unique_ptr<Scene> CreateNextScene() const; // Create the next scene. Called by SceneManager.

protected:
    std::function<std::unique_ptr<Scene>(Game*)> _NextSceneCreator; // Function to create next scene

private:
    // Helper for object registration
    std::shared_ptr<GameObject> RegisterGameObject(std::shared_ptr<GameObject> gameObject);
};
