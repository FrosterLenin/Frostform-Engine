#pragma once
#include "core/Player.hpp"
#include "core/enums/InputActions.hpp"
#include "spaceInvaders/Bullet.hpp"
#include <queue>
#include <memory>

class SpaceShip : public Player,  public std::enable_shared_from_this<SpaceShip>{
private:
    InputAction _UP;
    InputAction _DOWN;
    InputAction _LEFT;
    InputAction _RIGHT;
    InputAction _SHOOT;

    std::queue<std::shared_ptr<Bullet>> _Bullets;

    int _Life;

public:
    SpaceShip(Game* game, FVector2 position = FVector2{0,0}, FVector2 size = FVector2{1,1}
        , Color color = RAYWHITE, float accelerationIndex = 100.0f);
    ~SpaceShip();
    virtual void Update(float deltaTime) override;
    virtual void UpdateWithInput(float deltaTime) override;
    virtual void Start() override;
    virtual void Draw() override;
    virtual void OnCollisionEnter(FCollisionInfo& collisionInfo) override;

    void Shoot();
    void ReturnBullet(std::shared_ptr<Bullet> bullet);

    int GetLife() const;
    void TakeDamage(const int amount);

};