#pragma once
#include "core/Game.hpp"
#include "spaceInvaders/Bullet.hpp"

class SpaceInvadersGameScene;

class SpaceInvaders : public Game
{
public:
    SpaceInvaders();

    virtual void InitGame(const Color clearColor = BLACK) override;

    // Access to game scene
    SpaceInvadersGameScene* GetGameScene() const;
    
    // Set game scene (called by scene during Init)
    void SetGameScene(SpaceInvadersGameScene* scene);
    
    // Spawn bullet - delegates to game scene
    std::shared_ptr<Bullet> SpawnBullet(GameObject* owner, FVector2 position, float radius, Color color, float accelerationIndex, bool isPlayer = true);

private:
    SpaceInvadersGameScene* _GameScene;
};