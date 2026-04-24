#pragma once
#include "core/Game.hpp"
#include "spaceInvaders/Bullet.hpp"
#include <functional>

class SpaceInvaders : public Game
{
// public:
//     std::function<void()> GameOver;
public:
    SpaceInvaders();

    virtual void InitGame(const Color clearColor = BLACK) override;
    virtual void Update(float deltaTime) override;
    // virtual void Draw() override;
    std::shared_ptr<Bullet> SpawnBullet(GameObject* owner, FVector2 position, float radius, Color color, float accelerationIndex);

private:
    void InitEnemies(int row, int column, float spacing);
};