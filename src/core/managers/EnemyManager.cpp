#include "core/managers/EnemyManager.hpp"

EnemyManager::EnemyManager() {}
EnemyManager::~EnemyManager() {}

void EnemyManager::Init() {
    _Enemies.clear();
}
void EnemyManager::Bind(std::shared_ptr<GameObject> enemy){
    _Enemies.push_back(enemy);
}
void EnemyManager::Update(float deltaTime) {
    for(auto& enemy : _Enemies){
        if(auto sharedEnemy = enemy.lock()){
            if(sharedEnemy->IsActive()){
                sharedEnemy->UpdateAutomatic(deltaTime);
            }
        }
    }
}
bool EnemyManager::IsEmpty(){
    for(auto& enemy : _Enemies){
        if(auto sharedEnemy = enemy.lock()){
            if(sharedEnemy->IsActive()){
                return false;
            }
        }
    }
    return true;
}