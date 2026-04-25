#pragma once
#include "core/Game.hpp"
#include "core/Managers/UIManager.hpp"
#include "pong/ScoreUI.hpp"
#include <vector>
#include <memory>
#include <functional>

class PongGame : public Game{
private:
    int _P1Points;
    int _P2Points;
    float _EndGameDelay;
    bool _GameOver;
    std::unique_ptr<UIManager> _UIManager;
public:
    PongGame(FVector2 screenSize);

    virtual void Update(float deltaTime) override;
    virtual void Draw() override;
    virtual void InitGame(const Color clearColor = BLACK) override;

    int GetPlayerPoints(const int playerIndex) const;
    void ScorePoint(const int playerIndex);
    void DrawEndGameScreen();
private:
    void InitUI();
};