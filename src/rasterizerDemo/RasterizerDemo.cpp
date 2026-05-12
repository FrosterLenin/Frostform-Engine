#include "rasterizerDemo/RasterizerDemo.hpp"
#include "rasterizerDemo/RasterizerDemoScene.hpp"
#include "core/managers/SceneManager.hpp"

RasterizerDemo::RasterizerDemo(FVector2 screenSize)
    : Game(screenSize, "Frostform Rasterizer Demo")
{
}

void RasterizerDemo::InitGame(const Color clearColor)
{
    Game::InitGame(clearColor);
    _SceneManager->LoadScene<RasterizerDemoScene>();
}

RasterizerDemoScene* RasterizerDemo::GetGameScene() const
{
    return dynamic_cast<RasterizerDemoScene*>(_SceneManager->GetCurrentScene());
}
