#include "core/managers/BaseManager.hpp"

BaseManager::BaseManager() {}
BaseManager::~BaseManager() {}

void BaseManager::Bind(std::shared_ptr<GameObject> gameObject) {
    _ManagedObjects.push_back(gameObject);
}

void BaseManager::Init() {
    _ManagedObjects.clear();
}

void BaseManager::Update(float deltaTime) {
    for(auto& managedObject : _ManagedObjects) {
        if(auto sharedObject = managedObject.lock()) {
            if(sharedObject->IsActive()) {
                sharedObject->UpdateControlled(deltaTime);
            }
        }
    }
}

bool BaseManager::IsEmpty() {
    for(auto& managedObject : _ManagedObjects) {
        if(auto sharedObject = managedObject.lock()) {
            if(sharedObject->IsActive()) {
                return false;
            }
        }
    }
    return true;
}
