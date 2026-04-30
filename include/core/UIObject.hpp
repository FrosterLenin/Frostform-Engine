#pragma once
#include "core/GameObject.hpp"
#include <functional>

/**
 * Base template class for all UI objects (StatBar, ScoreUI, etc)
 * Each UIObject subclass specifies its own callback signature for UpdateEvent
 * 
 * Template parameter Func: the callback function signature
 * 
 * Example:
 *   using ScoreCallback = std::function<void(int, int)>;
 *   class ScoreUI : public UIObject<ScoreCallback> { };
 */
template<typename Func>
class UIObject : public GameObject
{
public:
    // UpdateEvent with the callback signature specified by the subclass
    Func UpdateEvent;

public:
    UIObject(Game* game, const FVector2 position = FVector2{0, 0}, 
             const FVector2 size = FVector2{1, 1}, 
             const Color color = RAYWHITE)
        : GameObject(game, position, size, color)
        , UpdateEvent(nullptr)
    {
    }

    virtual ~UIObject() = default;
};
