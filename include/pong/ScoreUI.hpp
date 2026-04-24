#pragma once
#include "core/GameObject.hpp"

class ScoreUI : public GameObject{
private:
    int _ScoreLeft;
    int _ScoreRight;
public:
    ScoreUI(Game* game, FVector2 position = FVector2{0,0}, FVector2 size = FVector2{1,1}, Color color = RAYWHITE);
    ~ScoreUI();
    
    virtual void Update(float deltaTime) override;
    // virtual void UpdateWithInput(float deltaTime) override;
    virtual void Draw() override;
    virtual void Start() override;

    virtual bool CheckCollision(const GameObject& other) const override;

    void UpdateScore(const int playerIndex);
};