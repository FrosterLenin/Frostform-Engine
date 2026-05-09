#pragma once

#include "core/Scene.hpp"

// The game over scene for Pong. Displays final score and winner.
class PongGameOverScene : public Scene
{
public:
    PongGameOverScene(Game* game, int player1Score, int player2Score);
    virtual ~PongGameOverScene() = default;

    virtual void Init() override;
    virtual SceneResult Update(float deltaTime) override;
    virtual void Draw() override;
    virtual void Destroy() override;

private:
    int _Player1Score;
    int _Player2Score;
    float _DisplayTime;
    static constexpr float DISPLAY_DURATION = 5.0f;
};
