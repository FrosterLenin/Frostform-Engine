#include "pong/PongGame.hpp"
#include "pong/PongGameScene.hpp"
#include "pong/PongGameOverScene.hpp"
#include "core/managers/SceneManager.hpp"

PongGame::PongGame(FVector2 screenSize) : Game(screenSize, "Pong"), _GameScene(nullptr), _GameOverScene(nullptr)
{
}

void PongGame::InitGame(const Color clearColor){
    _DrawManager->Init();
    SetClearColor(clearColor);
    
    // Load the game scene using SceneManager
    _SceneManager->LoadScene<PongGameScene>();
    _GameScene = dynamic_cast<PongGameScene*>(_SceneManager->GetCurrentScene());
    
    // Set the next scene (game over scene) that will be loaded when game scene completes
    if (_GameScene)
        _GameScene->SetNextScene<PongGameOverScene>(_GameScene);
}

PongGameScene* PongGame::GetGameScene() const
{
    return _GameScene;
}

void PongGame::SetGameScene(PongGameScene* scene)
{
    _GameScene = scene;
}

