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
    std::map<GameObject*, std::any> _Events;

public:
    UIManager();
    ~UIManager();

    void Init();
    virtual void Bind(std::shared_ptr<GameObject> gameObject) override;
    
    // Bind an event from a UI object to the manager (supports any number of parameters)
    template<typename... Args>
    void BindEvent(GameObject* UIObject, std::function<void(Args...)> event);
    
    // Trigger event for a specific UI object with parameters
    template<typename... Args>
    void TriggerObjectEvent(GameObject* UIObject, Args... args);
    
    // Trigger all bound events with parameters
    template<typename... Args>
    void TriggerAllEvents(Args... args);

    void SpawnUIObject(const FVector2& position, const FVector2& size, const Color color);
    void Update(float deltaTime);
    bool IsEmpty();
};