#pragma once
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include "utility/FVector2.hpp"
#include "core/GameObject.hpp"
#include "core/managers/InputManager.hpp"
#include "core/managers/DrawManager.hpp"
#include "core/managers/EnemyManager.hpp"
#include "core/managers/CollisionManager.hpp"
#include "core/managers/UIManager.hpp"
#include "core/managers/SceneManager.hpp"
#include "core/rasterizer/Screen.hpp"
#include "core/enums/RasterMode.hpp"

class Player; // Forward declaration
class SceneManager; // Forward declaration

class Game
{
protected:
    FVector2 _ScreenSize;
    std::string _Title;
    bool _ShouldClose;
    Color _ClearColor;
    std::vector<std::shared_ptr<GameObject>> _GameObjects;
    std::unique_ptr<CollisionManager> _CollisionManager;
    std::unique_ptr<InputManager> _InputManager;
    std::unique_ptr<DrawManager> _DrawManager;
    std::unique_ptr<EnemyManager> _EnemyManager;
    std::unique_ptr<UIManager> _UIManager;
    std::unique_ptr<SceneManager> _SceneManager;
    std::unique_ptr<Screen> _Screen;
    RasterMode _RasterMode;
public:
    const bool DEBUGMODE = true;
    Game(FVector2 screenSize, const std::string& title);
    ~Game();
    // NON COPYABLE CLASS
    Game(const Game& other) = delete;
    Game& operator=(const Game& other) = delete;

    void Run();
    void Quit();

    const FVector2 GetScreenSize() const;
    InputManager* GetInputManager() const;
    EnemyManager* GetEnemyManager() const;
    UIManager* GetUIManager() const;
    CollisionManager* GetCollisionManager() const;
    DrawManager* GetDrawManager() const;
    Screen* GetScreen() const;
    Color GetClearColor() const;
    RasterMode GetRasterMode() const;
    
    // Clear all managers (called when loading new scene)
    void ClearManagers();
    const std::vector<Player*> GetPlayers() const;
    void SetShouldClose(const bool shouldClose);
    void SetClearColor(const Color other);
    void SetRasterMode(const RasterMode mode);
    virtual void InitGame(const Color clearColor = BLACK);
    
    // TRY DONT USE THIS
    // This like a factory method to spawn game objects
    // it will be used by the game and the game objects to spawn new game objects
    // Template function to spawn game objects
    template<typename T, typename... Args>
    std::weak_ptr<T> SpawnGameObject(Args&&... args){
        if(!std::is_base_of<GameObject, T>::value) {
            throw std::invalid_argument("T must be a GameObject");
        }
        auto gameObject = std::make_shared<T>(std::forward<Args>(args)...);
        gameObject->SetActive(false); // Start inactive, the game object will activate itself when needed
        gameObject->Start();
        _GameObjects.push_back(gameObject);
        _CollisionManager.get()->RegisterCollider(gameObject);
        _DrawManager.get()->BindObject(gameObject);
        return gameObject;
    }

    // USE THIS INSTEAD
    std::shared_ptr<GameObject> SpawnGameObject(std::shared_ptr<GameObject> gameObject);

protected:
    virtual void Update(float deltaTime);
    virtual void Draw();
};
        