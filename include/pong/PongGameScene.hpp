#pragma once

#include "core/Scene.hpp"
#include "core/ScoreUI.hpp"

class Paddle;
class Ball;

/// @brief The active gameplay scene for Pong. Manages paddles, ball, and score tracking.
class PongGameScene : public Scene
{
public:
    PongGameScene(Game* game);
    virtual ~PongGameScene() = default;

    virtual void Init() override;
    virtual SceneResult Update(float deltaTime) override;
    virtual void Draw() override;
    virtual void Destroy() override;

    int GetPlayerScore(const int playerIndex) const;
    void ScorePoint(const int playerIndex, const int score);

    static constexpr int WINNING_SCORE = 10;

private:
    Paddle* _Player1;
    Paddle* _Player2;
    Ball* _Ball;
    ScoreUI* _ScoreUI;
    
    void InitUI();
};
