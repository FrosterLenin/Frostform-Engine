#pragma once
#include <memory>
#include <vector>
#include "core/GameObject.hpp"

class BaseManager{
protected:
    std::vector<std::weak_ptr<GameObject>> _ManagedObjects;

public:
    BaseManager();
    virtual ~BaseManager();

    virtual void Bind(std::shared_ptr<GameObject> gameObject);
    
    // Common methods with default implementations
    virtual void Init();
    virtual void Update(float deltaTime);
    virtual bool IsEmpty();
};