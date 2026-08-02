#pragma once
#include <vector>
#include <memory>
#include <functional>
#include <map>
#include <any>
#include "core/GameObject.hpp"
#include "core/managers/BaseManager.hpp"

class UIManager : public BaseManager{
private:
    // Use weak ownership as event key so stale raw pointers cannot outlive UI objects
    std::map<std::weak_ptr<GameObject>, std::any, std::owner_less<std::weak_ptr<GameObject>>> _Events;

public:
    UIManager();
    ~UIManager();

    void Init() override;
    
    // Bind an event from a UI object to the manager
    template<typename... Args>
    void BindEvent(const std::weak_ptr<GameObject>& UIObject, std::function<void(Args...)> event);
    
    // Trigger event for a UI object of type T with parameters
    template<typename T, typename... Args>
    void TriggerObjectEvent(Args... args) const;
    
    // Trigger all bound events with parameters
    template<typename... Args>
    void TriggerAllEvents(Args... args);

    // Get a managed UI object by type
    template<typename T>
    T* GetManagedObject() const;
};