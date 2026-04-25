#include "core/managers/UIManager.hpp"

UIManager::UIManager() {}
UIManager::~UIManager() {}

void UIManager::Init() {
    _ManagedObjects.clear();
    _Events.clear();
}

void UIManager::Bind(std::shared_ptr<GameObject> UIObject){
    _ManagedObjects.push_back(UIObject);
}

// Template implementations
template<typename... Args>
void UIManager::BindEvent(GameObject* UIObject, std::function<void(Args...)> event) {
    _Events[UIObject] = event;
}

template<typename... Args>
void UIManager::TriggerObjectEvent(GameObject* UIObject, Args... args) {
    auto iterator = _Events.find(UIObject);
    if (iterator != _Events.end()) {
        try {
            auto function = std::any_cast<std::function<void(Args...)>>(iterator->second);
            if (function) function(args...);
        } catch(...) {
            // Event doesn't match this signature, skip silently
        }
    }
}

template<typename... Args>
void UIManager::TriggerAllEvents(Args... args) {
    for (auto& eventPair : _Events) {
        try {
            auto function = std::any_cast<std::function<void(Args...)>>(eventPair.second);
            if (function) function(args...);
        } catch(...) {
            // Event doesn't match this signature, skip silently
        }
    }
}

// Explicit template instantiations for commonly used signatures
template void UIManager::BindEvent<int>(GameObject*, std::function<void(int)>);
template void UIManager::BindEvent<>(GameObject*, std::function<void()>);
template void UIManager::TriggerObjectEvent<int>(GameObject*, int);
template void UIManager::TriggerObjectEvent<>(GameObject*);
template void UIManager::TriggerAllEvents<int>(int);
template void UIManager::TriggerAllEvents<>();

void UIManager::Update(float deltaTime) {
    for(auto& UIObject : _ManagedObjects){
        if(auto sharedUIObject = UIObject.lock()){
            if(sharedUIObject->IsActive())
                sharedUIObject->UpdateControlled(deltaTime);
        }
    }
}

bool UIManager::IsEmpty(){
    for(auto& UIObject : _ManagedObjects){
        if(auto sharedUIObject = UIObject.lock()){
            if(sharedUIObject->IsActive())
                return false;
        }
    }
    return true;
}