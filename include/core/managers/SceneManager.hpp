#pragma once

#include <memory>
#include "core/Scene.hpp"
#include "core/enums/SceneResult.hpp"

class Game;

// Manages scene lifecycle and transitions. Owns the active scene and orchestrates transitions.
class SceneManager
{
private:
    Game* _Game;
    std::unique_ptr<Scene> _CurrentScene;
public:
    SceneManager(Game* game);
    ~SceneManager();

    // Load a new scene, unloading the current one if it exists
    template<typename T, typename... Args>
    void LoadScene(Args&&... args) {
        // Clear managers and input bindings before loading new scene
        ClearAllManagers();
        ClearInputBindings();
        
        // Unload current scene if it exists
        if (_CurrentScene) {
            _CurrentScene->Destroy();
            _CurrentScene.reset();
        }

        // Create new scene and set it as current BEFORE calling Init
        std::unique_ptr<Scene> newScene = std::make_unique<T>(_Game, std::forward<Args>(args)...);
        _CurrentScene = std::move(newScene);
        _CurrentScene->Init();
    }

    SceneResult Update(float deltaTime);
    void Draw();

    Scene* GetCurrentScene() const; // Get current active scene
    bool HasActiveScene() const;  // Check if a scene is loaded

private:
    void ClearInputBindings();  // Clear input bindings when loading new scene
    void ClearAllManagers();    // Clear all managers when loading new scene
};
