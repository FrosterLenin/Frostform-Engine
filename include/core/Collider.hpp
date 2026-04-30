#pragma once
#include "utility/FVector2.hpp"
#include "core/enums/CollisionLayers.hpp"
#include <Algorithm>
#include <memory>

class GameObject;

struct RectangleCollider;
struct CircleCollider;

struct FCollisionInfo{
public:
    FVector2 OverlapPoint;
    GameObject* OtherObject;

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

    // The layer(s) this collider belongs to. Combine with | to put it on multiple layers.
    CollisionLayer _Layer;
    // Bitmask of layers this collider is allowed to collide with. Combine with |.
    // Default: collide with everything.
    CollisionLayer _Mask;

    Collider(FVector2 position);
    Collider(FVector2 position, CollisionLayer layer, CollisionLayer mask);

    virtual void Draw() const = 0;

    // Returns true if A and B are configured to collide based on their layer/mask.
    // Both directions must allow the collision (A can hit B AND B can hit A).
    static bool CanCollide(const Collider& A, const Collider& B);

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
    RectangleCollider(FVector2 position, FVector2 size, CollisionLayer layer, CollisionLayer mask);
    void Draw() const override;
};
struct CircleCollider : public Collider{
    float _Radius;
    CircleCollider(FVector2 position, float radius);
    CircleCollider(FVector2 position, float radius, CollisionLayer layer, CollisionLayer mask);
    void Draw() const override;
};

