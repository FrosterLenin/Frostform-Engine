#include "spaceInvaders/SpaceInvaders.hpp"
#include "spaceInvaders/SpaceInvadersGameScene.hpp"
#include "spaceInvaders/Bullet.hpp"
#include "core/managers/SceneManager.hpp"

SpaceInvaders::SpaceInvaders() : Game({800, 600}, "Space Invaders")
{
}

void SpaceInvaders::InitGame(const Color clearColor){
    Game::InitGame(clearColor);
    // Load the game scene using SceneManager
    _SceneManager->LoadScene<SpaceInvadersGameScene>();
}

SpaceInvadersGameScene* SpaceInvaders::GetGameScene() const
{
    return dynamic_cast<SpaceInvadersGameScene*>(_SceneManager->GetCurrentScene());
}

std::shared_ptr<Bullet> SpaceInvaders::SpawnBullet(GameObject* owner, FVector2 position, float radius, Color color, float accelerationIndex, bool isPlayer)
{
    SpaceInvadersGameScene* scene = dynamic_cast<SpaceInvadersGameScene*>(_SceneManager->GetCurrentScene());
    if (scene)
        return scene->SpawnBullet(owner, position, radius, color, accelerationIndex, isPlayer);
    return nullptr;
}
