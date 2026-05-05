#include "spaceInvaders/SpaceInvaders.hpp"
#include "spaceInvaders/SpaceInvadersGameScene.hpp"
#include "spaceInvaders/Bullet.hpp"
#include "core/managers/SceneManager.hpp"

SpaceInvaders::SpaceInvaders() : Game({800, 600}, "Space Invaders"), _GameScene(nullptr)
{
}

void SpaceInvaders::InitGame(const Color clearColor){
    _DrawManager->Init();
    SetClearColor(clearColor);
    
    // Load the game scene using SceneManager
    _SceneManager->LoadScene<SpaceInvadersGameScene>();
    _GameScene = dynamic_cast<SpaceInvadersGameScene*>(_SceneManager->GetCurrentScene());
}

SpaceInvadersGameScene* SpaceInvaders::GetGameScene() const
{
    return _GameScene;
}

void SpaceInvaders::SetGameScene(SpaceInvadersGameScene* scene)
{
    _GameScene = scene;
}

std::shared_ptr<Bullet> SpaceInvaders::SpawnBullet(GameObject* owner, FVector2 position, float radius, Color color, float accelerationIndex, bool isPlayer)
{
    if (_GameScene)
        return _GameScene->SpawnBullet(owner, position, radius, color, accelerationIndex, isPlayer);
    return nullptr;
}
