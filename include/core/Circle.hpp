#pragma once
#include "core/GameObject.hpp"

class Circle : public GameObject
{
protected:
    float _Radius;
public:
    Circle(Game* game, FVector2 position = FVector2{0,0}, float radius = 0, Color color = RAYWHITE);
    Circle(Game* game, float accelerationIndex, FVector2 position = FVector2{0,0}, float radius = 0, Color color = RAYWHITE);
    ~Circle();
    virtual void Update(float deltaTime) override;
    virtual void Draw() override;
    virtual bool CheckCollision(const GameObject& other) const override;

    void SetRadius(const float other);
    float GetRadius() const;
};
