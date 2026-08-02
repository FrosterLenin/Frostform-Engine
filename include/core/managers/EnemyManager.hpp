#pragma once
#include <vector>
#include <memory>
#include "core/GameObject.hpp"
#include "core/managers/BaseManager.hpp"

class EnemyManager : public BaseManager{
public:
    EnemyManager();
    ~EnemyManager();
    const std::vector<std::weak_ptr<GameObject>>& GetManagedObjects() const { return _ManagedObjects; }
};