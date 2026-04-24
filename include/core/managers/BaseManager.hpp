#pragma once
#include <memory>
#include "core/GameObject.hpp"

class BaseManager{
public:
    BaseManager();
    virtual ~BaseManager();

    virtual void Bind(std::shared_ptr<GameObject> gameObject) = 0;
};