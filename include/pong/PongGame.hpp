#pragma once
#include "core/Game.hpp"

class PongGameScene;
class PongGameOverScene;

class PongGame : public Game{
public:
    static constexpr float BASE_ACCELERATION = 150.0f;
    static constexpr int WINNING_SCORE = 5; // Score needed to win the game
    PongGame(FVector2 screenSize = FVector2{800, 450});

    virtual void InitGame(const Color clearColor = BLACK) override;

    // Access to game scene for passing data between scenes
    PongGameScene* GetGameScene() const;
};