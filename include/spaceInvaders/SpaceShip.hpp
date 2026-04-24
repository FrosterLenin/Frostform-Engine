#pragma once
#include "core/GameObject.hpp"
#include "core/enums/InputActions.hpp"
#include "spaceInvaders/Bullet.hpp"
#include <queue>
#include <memory>

class SpaceShip : public GameObject,  public std::enable_shared_from_this<SpaceShip>{
private:
    InputAction _UP;
    InputAction _DOWN;
    InputAction _LEFT;
    InputAction _RIGHT;
    InputAction _SHOOT;

    std::queue<std::shared_ptr<Bullet>> _Bullets;
    
public:
    SpaceShip(Game* game, FVector2 position = FVector2{0,0}, FVector2 size = FVector2{1,1}
        , Color color = RAYWHITE, float accelerationIndex = 100.0f);
    ~SpaceShip();
    virtual void Update(float deltaTime) override;
    virtual void UpdateWithInput(float deltaTime) override;
    virtual void Start() override;
    virtual void Draw() override;

    void Shoot();
    void ReturnBullet(std::shared_ptr<Bullet> bullet);

};