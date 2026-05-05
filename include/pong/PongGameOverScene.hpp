#pragma once

#include "core/Scene.hpp"

class PongGameScene;

/// @brief The game over scene for Pong. Displays final score and winner.
class PongGameOverScene : public Scene
{
public:
    PongGameOverScene(Game* game, PongGameScene* gameScene);
    virtual ~PongGameOverScene() = default;

    virtual void Init() override;
    virtual SceneResult Update(float deltaTime) override;
    virtual void Draw() override;
    virtual void Destroy() override;

private:
    PongGameScene* _GameScene;
    float _DisplayTime;
    static constexpr float DISPLAY_DURATION = 5.0f;
};
