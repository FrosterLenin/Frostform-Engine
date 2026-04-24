#pragma once
#include "core/Circle.hpp"
#include "core/enums/InputActions.hpp"


class Ball : public Circle{
    private:
    bool _IsAutomatic; // If it works automatically or needs input
    int _RandomIndexVelocityX;
    int _RandomIndexVelocityY;
    int _Index;
    int _SpeedCap;
    InputAction _UP;
    InputAction _DOWN;
    InputAction _LEFT;
    InputAction _RIGHT;
public:
    Ball(Game* game, FVector2 position = FVector2{0,0}, float radius = 1.0f, Color color = RAYWHITE, bool isAutomatic = false, int index = 0);
    ~Ball();
    virtual void Update(float deltaTime) override;
    virtual void UpdateWithInput(float deltaTime) override;
    virtual void Start() override;

    virtual void OnCollisionEnter(FCollisionInfo& collisionInfo) override;

    void SetRandomIndexVelocityX(const int value);
    void SetRandomIndexVelocityY(const int value);
    void SetSpeedCap(const int value);
};