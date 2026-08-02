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

bool Bullet::IsOutOfVerticalBounds(const FVector2& nextPosition) const {
    return nextPosition.y - GetHalfHeight() < 0 || nextPosition.y + GetHalfHeight() > GetScreenHeight();
}

bool Bullet::IsOutOfHorizontalBounds(const FVector2& nextPosition) const {
    // Current Space Invaders bullets are intentionally vertical-only (X velocity is not used)
    // Keep horizontal expiry disabled until diagonal or horizontal projectile movement is introduced
    (void)nextPosition;
    return false;
}

void Bullet::Update(float deltaTime){
    if(!_Game) return;
    FVector2 nextPosition = GetCenter() + _Velocity.Normalized() * _AccelerationIndex * deltaTime;
    if(IsOutOfVerticalBounds(nextPosition) || IsOutOfHorizontalBounds(nextPosition)){
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