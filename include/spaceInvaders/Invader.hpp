#pragma once
#include "core/GameObject.hpp"

class Invader : public GameObject{

public:
    Invader(Game* game, FVector2 position = FVector2{0,0}, FVector2 size = FVector2{1,1}
        , Color color = RAYWHITE, float accelerationIndex = 50.0f);
    ~Invader();

    virtual void Update(float deltaTime) override;
    virtual void Start() override;
    virtual void Draw() override;

    virtual void UpdateControlled(float deltaTime) override;
    virtual void OnCollisionEnter(FCollisionInfo& collisionInfo) override;
};