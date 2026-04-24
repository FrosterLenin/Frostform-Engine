#pragma once
#include "utility/FVector2.hpp"
#include <Algorithm>
#include <memory>

class GameObject;

struct RectangleCollider;
struct CircleCollider;

struct FCollisionInfo{
public:
    FVector2 OverlapPoint;

    FCollisionInfo();
};

struct FCollisionPair{
public:
    std::weak_ptr<GameObject> A;
    std::weak_ptr<GameObject> B;
    FCollisionPair(std::weak_ptr<GameObject> a, std::weak_ptr<GameObject> b);
    bool operator==(const FCollisionPair& other) const;
};

struct Collider{
public:
    FVector2 _Position;
    // bool _IsColliding;
    Collider(FVector2 position);

    virtual void Draw() const = 0;

    static bool CheckCollision(const RectangleCollider& A, const RectangleCollider& B, FCollisionInfo& collisionInfo);
    static bool CheckCollision(const CircleCollider& A, const CircleCollider& B, FCollisionInfo& collisionInfo);
    static bool CheckCollision(const RectangleCollider& R, const CircleCollider& C, FCollisionInfo& collisionInfo);
    static bool CheckCollision(const CircleCollider& C, const RectangleCollider& R, FCollisionInfo& collisionInfo);

public:
    virtual ~Collider() = default; // Polymorfic so we can dynamic cast a Collider (we explicitely say that it has more forms)
};

struct RectangleCollider : public Collider{
    FVector2 _Size;
    RectangleCollider(FVector2 position, FVector2 size);
    void Draw() const override;
};
struct CircleCollider : public Collider{
    float _Radius;
    CircleCollider(FVector2 position, float radius);
    void Draw() const override;
};

