#pragma once
#include "core/GameObject.hpp"
#include "spaceInvaders/Bullet.hpp"
#include <queue>
#include <memory>

class Invader : public GameObject, public std::enable_shared_from_this<Invader>{
private:
    // Pool of bullets to shoot, we use a queue to reuse the bullets and avoid creating new ones every time
    std::queue<std::shared_ptr<Bullet>> _Bullets;
    static constexpr int MAX_BULLETS = 3; // Max bullets in the pool, can be tweaked for performance/behavior
public:
    static constexpr int COLLISION_DAMAGE = 10;
    static constexpr float SHOOT_INTERVAL = 2.0f; // Time between shots in seconds
    static float _TimeSinceLastShot; // Timer to track shooting intervals
    
    Invader(Game* game, FVector2 position = FVector2{0,0}, FVector2 size = FVector2{1,1}
        , Color color = RAYWHITE, float accelerationIndex = 50.0f);
    ~Invader();

    virtual void Update(float deltaTime) override;
    virtual void Start() override;
    virtual void Draw() override;

    virtual void UpdateControlled(float deltaTime) override;
    virtual void OnCollisionEnter(FCollisionInfo& collisionInfo) override;

    void Shoot();
    void ReturnBullet(std::shared_ptr<Bullet> bullet);
};