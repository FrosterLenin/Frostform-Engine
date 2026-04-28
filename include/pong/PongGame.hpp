#pragma once
#include "core/Game.hpp"
#include "core/ScoreUI.hpp"
#include <vector>
#include <memory>
#include <functional>

class Paddle;

class PongGame : public Game{
private:
    Paddle* _Player1;
    Paddle* _Player2;
    float _EndGameDelay;
    bool _GameOver;
public:
    PongGame(FVector2 screenSize);

    virtual void Update(float deltaTime) override;
    virtual void Draw() override;
    virtual void InitGame(const Color clearColor = BLACK) override;

    int GetPlayerScore(const int playerIndex) const;
    void ScorePoint(const int playerIndex, const int score);
    void DrawEndGameScreen();
private:
    void InitUI();
};