#include "core/managers/UIManager.hpp"
#include "core/ScoreUI.hpp"
#include "core/StatBar.hpp"

UIManager::UIManager() {}
UIManager::~UIManager() {}

void UIManager::Init() {
    BaseManager::Init();
    _Events.clear();
}

// Template implementations
template<typename... Args>
void UIManager::BindEvent(GameObject* UIObject, std::function<void(Args...)> event) {
    _Events[UIObject] = event;
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

// Trigger event for a UI object of type T with parameters
template<typename T, typename... Args>
void UIManager::TriggerObjectEvent(Args... args) const {
    // Search through _Events for an object of type T and trigger its event
    if(_Events.empty()) return;
    for (auto& eventPair : _Events) {
        T* gameObject = dynamic_cast<T*>(eventPair.first);
        if (gameObject) {
            try {
                auto function = std::any_cast<std::function<void(Args...)>>(eventPair.second);
                if (function) function(args...);
            } catch(...) {
                // Event doesn't match this signature, skip silently
            }
            break; // Found and triggered the event for this type
        }
    }
}

// Get a managed UI object by type
template<typename T>
T* UIManager::GetManagedObject() const {
    for (auto& managedObject : _ManagedObjects) {
        if (std::shared_ptr<GameObject> gameObject = managedObject.lock()) {
            // Attempt to dynamic cast to the requested type
            T* castedObject = dynamic_cast<T*>(gameObject.get());
            // If the cast is successful, return the pointer, otherwise continue searching
            if (castedObject) return castedObject;
        }
    }
    return nullptr; // No object of the requested type found
}

// Explicit template instantiations for commonly used signatures
// We use this instead of implementing template function in the header 
// since we want a lightweight system for all the engine events
// By explicitly instantiating only the signatures we use
// we can keep the implementation in the cpp file and avoid unnecessary code generation for unused event signatures
// This way we can have a more efficient event system while still maintaining flexibility for different event signatures when needed
// so we want to avoid the overhead of std::any_cast and std::function for every event trigger
// and instead only use it for the events we actually use in our games
// which are mostly void() and void(int) or void(int,int) for score updates and similar events
template void UIManager::BindEvent<int>(GameObject*, std::function<void(int)>);
template void UIManager::BindEvent<int, int>(GameObject*, std::function<void(int,int)>);
template void UIManager::BindEvent<>(GameObject*, std::function<void()>);

template void UIManager::TriggerAllEvents<int>(int);
template void UIManager::TriggerAllEvents<>();


// This is needed to avoid linker errors for the ScoreUI type, which is used in our games
template ScoreUI* UIManager::GetManagedObject<ScoreUI>() const;
template void UIManager::TriggerObjectEvent<ScoreUI>(int, int) const;

// This is needed to avoid linker errors for the StatBar type, which is used in Space Invaders
template StatBar* UIManager::GetManagedObject<StatBar>() const;
template void UIManager::TriggerObjectEvent<StatBar>(int) const;