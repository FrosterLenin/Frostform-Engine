#include "core/managers/CollisionManager.hpp"

void CollisionManager::RegisterCollider(std::shared_ptr<GameObject> gameOject){
    if(gameOject.get()->GetCollider() != nullptr)
        _GameObjects.push_back(gameOject);
}
void CollisionManager::UnregisterCollider(std::shared_ptr<GameObject> gameOject){
    for(size_t i = 0; i < _GameObjects.size(); ++i){
        std::shared_ptr<GameObject> gameObjectPointer = _GameObjects.at(i).lock();
        if(gameObjectPointer == nullptr) continue;
        if(gameObjectPointer == gameOject){
            _GameObjects.erase(_GameObjects.begin() + i);
            return;
        }
    }
}
void CollisionManager::Update(){
    _PreviousCollisions = _CurrentCollisions;
    _CurrentCollisions.clear();

    for(size_t i = 0; i < _GameObjects.size(); ++i){
        std::shared_ptr<GameObject> gameObjectA = _GameObjects.at(i).lock();
        if(!gameObjectA || !gameObjectA.get()->IsActive()) continue;
        for(size_t j = i+1; j < _GameObjects.size(); ++j){
            
            std::shared_ptr<GameObject> gameObjectB = _GameObjects.at(j).lock();
            if(!gameObjectB || !gameObjectB.get()->IsActive()) continue;

            FCollisionInfo collisionInfo;
            if(CheckForCollissionPair(gameObjectA.get()->GetCollider(), gameObjectB.get()->GetCollider(), collisionInfo)){

                _CurrentCollisions.emplace_back(gameObjectA, gameObjectB);
                if(!HasCollisionPair(_PreviousCollisions, gameObjectA, gameObjectB)){
                    gameObjectA->OnCollisionEnter(collisionInfo);
                    gameObjectB->OnCollisionEnter(collisionInfo);
                }
                else{
                    gameObjectA->OnCollisionStay(collisionInfo);
                    gameObjectB->OnCollisionStay(collisionInfo);
                }
            }
        }
    }

    for(const auto& collisionPair : _PreviousCollisions){
        std::shared_ptr<GameObject> gameObjectA = collisionPair.A.lock();
        std::shared_ptr<GameObject> gameObjectB = collisionPair.B.lock();
        if(!gameObjectA || !gameObjectB) continue;

        if(!HasCollisionPair(_CurrentCollisions, gameObjectA, gameObjectB)){
            FCollisionInfo collisionInfo; // We can pass an empty collision info since the collision is already ended
            gameObjectA->OnCollisionExit(collisionInfo);
            gameObjectB->OnCollisionExit(collisionInfo);
        }
    }
}
bool CollisionManager::CheckForCollissionPair(const Collider* A, const Collider* B, FCollisionInfo& collisionInfo){
    const RectangleCollider* rectangleColliderA = dynamic_cast<const RectangleCollider*>(A);
    const RectangleCollider* rectangleColliderB = dynamic_cast<const RectangleCollider*>(B);
    
    if(rectangleColliderA != nullptr && rectangleColliderB != nullptr)
        return Collider::CheckCollision(*rectangleColliderA, *rectangleColliderB, collisionInfo);

    const CircleCollider* circleColliderA = dynamic_cast<const CircleCollider*>(A);
    const CircleCollider* circleColliderB = dynamic_cast<const CircleCollider*>(B);

    if(circleColliderA != nullptr && circleColliderB != nullptr)
        return Collider::CheckCollision(*circleColliderA, *circleColliderB, collisionInfo);
    else if(rectangleColliderA != nullptr && circleColliderB != nullptr)
        return Collider::CheckCollision(*rectangleColliderA, *circleColliderB, collisionInfo);
    else if(circleColliderA != nullptr && rectangleColliderB != nullptr)
        return Collider::CheckCollision(*circleColliderA, *rectangleColliderB, collisionInfo);
    return false;
}
bool CollisionManager::HasCollisionPair(const std::vector<FCollisionPair>& pair, const std::shared_ptr<GameObject>& A, const std::shared_ptr<GameObject>& B){
    if(!A || ! B) return false;

    FCollisionPair collisionPair(A, B);
    // This calls the operator == of FCollisionPair
    return std::find(pair.begin(), pair.end(), collisionPair) != pair.end();
}