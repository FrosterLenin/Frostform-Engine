#include "core/managers/EnemyManager.hpp"

EnemyManager::EnemyManager() {}
EnemyManager::~EnemyManager() {}

void EnemyManager::Init() {
    _ManagedObjects.clear();
}
void EnemyManager::Bind(std::shared_ptr<GameObject> enemy){
    _ManagedObjects.push_back(enemy);
}
void EnemyManager::Update(float deltaTime) {
    for(auto& enemy : _ManagedObjects){
        if(auto sharedEnemy = enemy.lock()){
            if(sharedEnemy->IsActive()){
                sharedEnemy->UpdateControlled(deltaTime);
            }
        }
    }
}
bool EnemyManager::IsEmpty(){
    for(auto& enemy : _ManagedObjects){
        if(auto sharedEnemy = enemy.lock()){
            if(sharedEnemy->IsActive()){
                return false;
            }
        }
    }
    return true;
}