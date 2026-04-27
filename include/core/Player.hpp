#pragma once
#include "core/GameObject.hpp"

class Player : public GameObject {
protected:
    int _Score;

public:
    Player(Game* game, FVector2 position = FVector2{0, 0}, FVector2 size = FVector2{1, 1}, Color color = RAYWHITE);
    Player(Game* game, FVector2 position, FVector2 size, Color color, float accelerationIndex);
    virtual ~Player() = default;

    // Score management
    virtual int GetScore() const;
    virtual void AddScore(int points = 1);
    virtual void SetScore(int score);
    virtual void ResetScore();
};
