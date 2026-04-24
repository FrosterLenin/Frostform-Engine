#pragma once
#include "core/GameObject.hpp"
#include "core/enums/InputActions.hpp"

class Paddle : public GameObject{
private:
    bool _IsAutomatic; // If it works automatically or needs input
    int _RandomIndexVelocityX;
    int _Index;
    InputAction _UP;
    InputAction _DOWN;
    InputAction _LEFT;
    InputAction _RIGHT;

public:
    Paddle(Game* game, FVector2 position = FVector2{0,0}, FVector2 size = FVector2{1,1}
        , Color color = RAYWHITE, bool isAutomatic = false, int index = 0);
    ~Paddle();
    virtual void Update(float deltaTime) override;
    virtual void UpdateWithInput(float deltaTime) override;
    virtual void Start() override;
    virtual void Draw() override;

    virtual bool CheckCollision(const GameObject& other) const override;

    void SetRandomIndexVelocityX(const int value);
};