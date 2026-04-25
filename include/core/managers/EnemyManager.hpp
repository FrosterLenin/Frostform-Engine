#pragma once
#include <vector>
#include <memory>
#include "core/GameObject.hpp"
#include "core/managers/BaseManager.hpp"

class EnemyManager : public BaseManager{
public:
    EnemyManager();
    ~EnemyManager();

    void Init();
    virtual void Bind(std::shared_ptr<GameObject> gameObject) override;
    void SpawnEnemy(const FVector2& position, const FVector2& size, const Color color, float accelerationIndex);
    void Update(float deltaTime);
    bool IsEmpty();
};