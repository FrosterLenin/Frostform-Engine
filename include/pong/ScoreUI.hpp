#pragma once
#include "core/GameObject.hpp"
#include <functional>

class ScoreUI : public GameObject{
private:
    int _ScoreLeft;
    int _ScoreRight;
public:
    std::function<void(int)> ScoreEvent;
public:
    ScoreUI(Game* game, FVector2 position = FVector2{0,0}, FVector2 size = FVector2{1,1}, Color color = RAYWHITE);
    ~ScoreUI();
    
    virtual void Update(float deltaTime) override;
    virtual void UpdateControlled(float deltaTime) override;
    virtual void Draw() override;
    virtual void Start() override;

    virtual bool CheckCollision(const GameObject& other) const override;

    void UpdateScore(const int playerIndex);
};