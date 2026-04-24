#pragma once
#include "core/enums/InputActions.hpp"
#include "utility/FVector2.hpp"
#include <array>

class InputManager{
private:
    static constexpr size_t ACTION_COUNT = static_cast<size_t>(InputAction::LAST);
    std::array<int, ACTION_COUNT> _KeyBindings;
    std::array<bool, ACTION_COUNT> _KeyDowns;
    std::array<bool, ACTION_COUNT> _KeyUps;
    std::array<bool, ACTION_COUNT> _KeyStates;
    FVector2 _MousePosition;
    FVector2 _MousePositionDelta;
    float _MouseWheel;

    size_t ToIndex(InputAction action) const;

public:
    InputManager();

    void BindKey(InputAction action, const int key);
    void UnbindKey(InputAction action);
    void Update();

    bool GetActionDown(InputAction action) const;
    bool GetActionUp(InputAction action) const;
    bool GetActionState(InputAction action) const;

    FVector2 GetInputMousePosition() const;
    FVector2 GetInputMouseDelta() const;
    float GetInputMouseWheel() const;

    bool GetMouseButtonDown(int button) const;
    bool GetMouseButtonUp(int button) const;
    bool GetMouseState(int button) const;
};