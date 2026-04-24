#pragma once
#include <vector>
#include <memory>
#include "core/GameObject.hpp"
#include "core/Collider.hpp"

class CollisionManager{
private:
    std::vector<std::weak_ptr<GameObject>> _GameObjects;
    std::vector<FCollisionPair> _CurrentCollisions;
    std::vector<FCollisionPair> _PreviousCollisions;
public:
    void RegisterCollider(std::shared_ptr<GameObject> gameObject);
    void UnregisterCollider(std::shared_ptr<GameObject> gameObject);
    void Update();

private:
    bool CheckForCollissionPair(const Collider* A, const Collider* B, FCollisionInfo& collisionInfo);
    bool HasCollisionPair(const std::vector<FCollisionPair>& pair, const std::shared_ptr<GameObject>& A, const std::shared_ptr<GameObject>& B);
};