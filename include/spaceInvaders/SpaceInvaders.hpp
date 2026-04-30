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
    std::shared_ptr<Bullet> SpawnBullet(GameObject* owner, FVector2 position, float radius, Color color, float accelerationIndex, bool isPlayer = true);

private:
    void ScorePoint(const int playerIndex, const int score);
    void InitEnemies(int row, int column, float spacing);
    void InitUI();
    // Ticks the shared invader shoot timer and fires from a random active invader when the interval is reached
    void TickInvaderShot(float deltaTime);
};