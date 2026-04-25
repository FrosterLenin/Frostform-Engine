#pragma once
#include <vector>
#include <memory>
#include "core/GameObject.hpp"
#include "core/managers/BaseManager.hpp"

class EnemyManager : public BaseManager{
public:
    EnemyManager();
    ~EnemyManager();

    void SpawnEnemy(const FVector2& position, const FVector2& size, const Color color, float accelerationIndex);
};