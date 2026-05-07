#include "rasterizerDemo/RasterizerDemo.hpp"
#include "rasterizerDemo/RasterizerDemoScene.hpp"
#include "core/managers/SceneManager.hpp"

RasterizerDemo::RasterizerDemo(FVector2 screenSize)
    : Game(screenSize, "Frostform Rasterizer Demo"), _GameScene(nullptr)
{
}


void RasterizerDemo::InitGame(const Color clearColor)
{
    _DrawManager->Init();
    SetClearColor(clearColor);

    _SceneManager->LoadScene<RasterizerDemoScene>();
    _GameScene = dynamic_cast<RasterizerDemoScene*>(_SceneManager->GetCurrentScene());
}

RasterizerDemoScene* RasterizerDemo::GetGameScene() const
{
    return _GameScene;
}

void RasterizerDemo::SetGameScene(RasterizerDemoScene* scene)
{
    _GameScene = scene;
}
