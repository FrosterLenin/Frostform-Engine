#include "spaceInvaders/Bullet.hpp"
#include "spaceInvaders/SpaceShip.hpp"

Bullet::Bullet(Game* game, GameObject* owner, FVector2 position, float radius, Color color, float accelerationIndex, FVector2 velocity) : 
    Circle(game, accelerationIndex, position, radius, color){
    _Owner = owner;
    _Velocity = velocity;
    _Collider = std::make_shared<CircleCollider>(position, radius);
}
Bullet::~Bullet(){}

void Bullet::Draw(){
    Circle::Draw();
}
void Bullet::Start(){
}
void Bullet::Update(float deltaTime){
    if(!_Game) return;
    FVector2 nextPosition = GetCenter() + _Velocity.Normalized() * _AccelerationIndex * deltaTime;
    if(nextPosition.y - GetHalfHeight() < 0 || nextPosition.y + GetHalfHeight() > GetScreenHeight()){
        // Preferred path: the owner callback handles pooling/deactivation; fallback only if no callback is bound
        if(OnExpired)
            OnExpired(shared_from_this());
        else
            SetActive(false);
        return;
    }
    SetPosition(_Position + _Velocity.Normalized() * _AccelerationIndex * deltaTime);
}
void Bullet::OnCollisionEnter(FCollisionInfo& collisionInfo){
    GameObject::OnCollisionEnter(collisionInfo);
    // Preferred path: the owner callback handles pooling/deactivation; fallback only if no callback is bound.
    if(OnExpired)
        OnExpired(shared_from_this());
    else
        SetActive(false);
}