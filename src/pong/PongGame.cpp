#include "pong/PongGame.hpp"
#include "pong/PongGameScene.hpp"
#include "pong/PongGameOverScene.hpp"
#include "core/managers/SceneManager.hpp"

PongGame::PongGame(FVector2 screenSize) 
: Game(screenSize, "Pong"), _GameScene(nullptr), _GameOverScene(nullptr)
{
}

void PongGame::InitGame(const Color clearColor){
    Game::InitGame(clearColor);
    
    // Load the game scene using SceneManager
    _SceneManager->LoadScene<PongGameScene>();
    _GameScene = dynamic_cast<PongGameScene*>(_SceneManager->GetCurrentScene());
}

PongGameScene* PongGame::GetGameScene() const
{
    return _GameScene;
}

void PongGame::SetGameScene(PongGameScene* scene)
{
    _GameScene = scene;
}

