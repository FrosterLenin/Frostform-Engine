#pragma once

#include "core/Scene.hpp"
#include "core/ScoreUI.hpp"
#include "core/StatBar.hpp"

class SpaceShip;
class Bullet;

// gameplay scene for Space Invaders. Manages ship, enemies, and bullet spawning.
class SpaceInvadersGameScene : public Scene
{
public:
    SpaceInvadersGameScene(Game* game);
    virtual ~SpaceInvadersGameScene() = default;

    virtual void Init() override;
    virtual SceneResult Update(float deltaTime) override;
    virtual void Draw() override;
    virtual void Destroy() override;

    std::shared_ptr<Bullet> SpawnBullet(GameObject* owner, FVector2 position, float radius, Color color, 
                                        float accelerationIndex, bool isPlayer = true);

private:
    SpaceShip* _Player;
    ScoreUI* _ScoreUI;
    StatBar* _LifeBarUI;
    
    void InitEnemies(int row, int column, float spacing);
    void InitUI();
    void ScorePoint(const int playerIndex, const int score);
    void TickInvaderShot(float deltaTime);
};
