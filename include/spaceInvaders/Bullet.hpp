#pragma once
#include "core/Circle.hpp"
#include <functional>
#include <memory>

class Bullet: public Circle, public std::enable_shared_from_this<Bullet>{
public:
    static constexpr int DAMAGE = 10;
public:
    std::function<void(std::shared_ptr<Bullet>)> OnExpired;
    Bullet(Game* game, GameObject* owner, FVector2 position = FVector2{0,0}, float radius = 10.0f
        , Color color = RAYWHITE, float accelerationIndex = 200.0f, FVector2 velocity = FVector2{0, -1});
    ~Bullet();


    virtual void Update(float deltaTime) override;
    virtual void Start() override;
    virtual void Draw() override;

    virtual void OnCollisionEnter(FCollisionInfo& collisionInfo) override;
    
    // This function reset bullet and put its position outside the game bounds
    // void ResetBullet(); 
};