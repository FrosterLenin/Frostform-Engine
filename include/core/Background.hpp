#pragma once
#include "core/GameObject.hpp"
class Background : public GameObject{
public:
    Background(Game* game, const FVector2 position = (.0f, .0f), Color color = BLACK);

    virtual void Update(float deltaTime) override;
    virtual void Start() override;
    virtual void Draw() override;
};