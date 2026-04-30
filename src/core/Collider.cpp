#include "core/Collider.hpp"
#include "raylib.h"

Collider::Collider(FVector2 position)
    : _Position(position), _Layer(CollisionLayer::DEFAULT), _Mask(CollisionLayer::ALL){
}
Collider::Collider(FVector2 position, CollisionLayer layer, CollisionLayer mask)
    : _Position(position), _Layer(layer), _Mask(mask){
}
CircleCollider::CircleCollider(FVector2 position, float radius)
    : Collider(position), _Radius(radius){
}
CircleCollider::CircleCollider(FVector2 position, float radius, CollisionLayer layer, CollisionLayer mask)
    : Collider(position, layer, mask), _Radius(radius){
}
RectangleCollider::RectangleCollider(FVector2 position, FVector2 size)
    : Collider(position), _Size(size){
}
RectangleCollider::RectangleCollider(FVector2 position, FVector2 size, CollisionLayer layer, CollisionLayer mask)
    : Collider(position, layer, mask), _Size(size){
}

bool Collider::CanCollide(const Collider& A, const Collider& B){
    // Both sides must accept the other's layer.
    return HasLayer(A._Mask, B._Layer) && HasLayer(B._Mask, A._Layer);
}

void RectangleCollider::Draw() const {
    DrawRectangleLinesEx({ _Position.x, _Position.y, _Size.x, _Size.y }, 1.0f, RED);
}

void CircleCollider::Draw() const {
    FVector2 center = _Position + FVector2{_Radius, _Radius};
    DrawCircleLines((int)center.x, (int)center.y, _Radius, RED);
}

bool Collider::CheckCollision(const RectangleCollider& A, const RectangleCollider& B, FCollisionInfo& collisionInfo){
    collisionInfo.OverlapPoint.x = A._Position.x - B._Position.x;
    collisionInfo.OverlapPoint.y = A._Position.y - B._Position.y;
    return A._Position.x < B._Position.x + B._Size.x
        && A._Position.x + A._Size.x > B._Position.x
        && A._Position.y < B._Position.y + B._Size.y
        && A._Position.y + A._Size.y > B._Position.y;
}
bool Collider::CheckCollision(const CircleCollider& A, const CircleCollider& B, FCollisionInfo& collisionInfo){
    FVector2 centerA = A._Position + FVector2{A._Radius, A._Radius};
    FVector2 centerB = B._Position + FVector2{B._Radius, B._Radius};
    return FVector2::Distance(centerA, centerB) < A._Radius + B._Radius;
}
bool Collider::CheckCollision(const RectangleCollider& R, const CircleCollider& C, FCollisionInfo& collisionInfo){
    FVector2 circleCenter = C._Position + FVector2{C._Radius, C._Radius};
    FVector2 closestPointPosition = {
        std::max(R._Position.x, std::min(circleCenter.x, R._Position.x + R._Size.x)),
        std::max(R._Position.y, std::min(circleCenter.y, R._Position.y + R._Size.y))
    };
    collisionInfo.OverlapPoint.x = C._Radius - std::abs(circleCenter.x - closestPointPosition.x);
    collisionInfo.OverlapPoint.y = C._Radius - std::abs(circleCenter.y - closestPointPosition.y);
    FVector2 delta = {
        circleCenter.x - closestPointPosition.x,
        circleCenter.y - closestPointPosition.y
    };
    return delta.Magnitude() < C._Radius;
}
bool Collider::CheckCollision(const CircleCollider& C, const RectangleCollider& R, FCollisionInfo& collisionInfo){
    return CheckCollision(R, C, collisionInfo);
}

FCollisionInfo::FCollisionInfo() : OverlapPoint(0,0), OtherObject(nullptr) {}

FCollisionPair::FCollisionPair(std::weak_ptr<GameObject> a, std::weak_ptr<GameObject> b): A(a), B(b) {}

bool FCollisionPair::operator==(const FCollisionPair& other) const{

    std::shared_ptr<GameObject> sharedA = A.lock();
    std::shared_ptr<GameObject> sharedB = B.lock();
    std::shared_ptr<GameObject> sharedOtherA = other.A.lock();
    std::shared_ptr<GameObject> sharedOtherB = other.B.lock();  
    // If any of the game objects is already destroyed
    // we consider that the collision pair is not valid and thus not equal to any other pair
    if(!sharedA || !sharedB || !sharedOtherA || !sharedOtherB) return false; 
    
    return 
        (sharedA == sharedOtherA && sharedB == sharedOtherB) 
        || (sharedA == sharedOtherB && sharedB == sharedOtherA);
}