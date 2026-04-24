#include "spaceInvaders/Bullet.hpp"
#include "spaceInvaders/SpaceShip.hpp"

Bullet::Bullet(Game* game, GameObject* owner, FVector2 position, float radius, Color color, float accelerationIndex) : 
    Circle(game, accelerationIndex, position, radius, color)
    , _Owner(owner) {
    _Collider = std::make_shared<CircleCollider>(position, radius);
}
Bullet::~Bullet(){}

void Bullet::Draw(){
    Circle::Draw();
}
void Bullet::Start(){
    _Velocity = {.0f, -1.0f}; // Move only in Y axis
    _AccelerationIndex = 200.0f;
}
void Bullet::Update(float deltaTime){
    if(!_Game) return;
    FVector2 nextPosition = GetCenter() + _Velocity.Normalized() * _AccelerationIndex * deltaTime;
    if(nextPosition.y - GetHalfHeight() < 0 || nextPosition.y + GetHalfHeight() > GetScreenHeight()){
        if(OnExpired)
            OnExpired(shared_from_this());
        return;
    }
    SetPosition(_Position + _Velocity.Normalized() * _AccelerationIndex * deltaTime);
}
void Bullet::OnCollisionEnter(FCollisionInfo& collisionInfo){
    GameObject::OnCollisionEnter(collisionInfo);
    if(OnExpired)
        OnExpired(shared_from_this());
}