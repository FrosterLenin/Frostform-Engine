#include "pong/PongGame.hpp"
#include "pong/PongGameScene.hpp"
#include "pong/PongGameOverScene.hpp"
#include "core/managers/SceneManager.hpp"

PongGame::PongGame(FVector2 screenSize, RasterMode rasterMode)
    : Game(screenSize, "Pong")
    , _PongRasterMode(rasterMode)
{
}

void PongGame::InitGame(const Color clearColor){
    Game::InitGame(clearColor);
    SetRasterMode(_PongRasterMode);
    // Load the game scene using SceneManager
    _SceneManager->LoadScene<PongGameScene>();
}

PongGameScene* PongGame::GetGameScene() const
{
    return dynamic_cast<PongGameScene*>(_SceneManager->GetCurrentScene());
}

