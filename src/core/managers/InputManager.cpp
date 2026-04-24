#include "core/managers/InputManager.hpp"

InputManager::InputManager(){
    for(auto& keyBinding : _KeyBindings){
        keyBinding = -1;
    }
    _KeyDowns.fill(false);
    _KeyUps.fill(false);
    _KeyStates.fill(false);

    _MousePosition = {0,0};
    _MousePositionDelta = {0,0};
    _MouseWheel = .0f;
}
size_t InputManager::ToIndex(InputAction action) const{
    return static_cast<size_t>(action);
}
void InputManager::BindKey(InputAction action, const int key){
    _KeyBindings[ToIndex(action)] = key;
}
void InputManager::UnbindKey(InputAction action){
    _KeyBindings[ToIndex(action)] = -1;
    _KeyDowns[ToIndex(action)] = false;
    _KeyUps[ToIndex(action)] = false;
    _KeyStates[ToIndex(action)] = false;
}
void InputManager::Update(){
    _KeyDowns.fill(false);
    _KeyUps.fill(false);

    for(size_t i = 0; i < ACTION_COUNT; ++i){
        const size_t key = _KeyBindings[i];
        if(key == -1) continue;
        bool isKeyDown = IsKeyDown(key);
        _KeyDowns[i] = isKeyDown && !_KeyStates[i];
        _KeyUps[i] = !isKeyDown && _KeyStates[i];
        _KeyStates[i] = isKeyDown; // IF STILL BEEING PRESSED
    }
    const FVector2 oldMousePosition = _MousePosition;
    const FVector2 currentMousePosition = GetInputMousePosition();
    _MousePosition = {currentMousePosition.x, currentMousePosition.y};
    _MousePositionDelta = _MousePosition - oldMousePosition;

}
bool InputManager::GetActionDown(InputAction action) const{
    return _KeyDowns[ToIndex(action)];
}
bool InputManager::GetActionUp(InputAction action) const{
    return _KeyUps[ToIndex(action)];
}
bool InputManager::GetActionState(InputAction action) const{
    return _KeyStates[ToIndex(action)];
}
FVector2 InputManager::GetInputMousePosition() const{
    return _MousePosition;
}
FVector2 InputManager::GetInputMouseDelta() const{
    return _MousePositionDelta;
}
float InputManager::GetInputMouseWheel() const{
    return _MouseWheel;
}
bool InputManager::GetMouseButtonDown(int button) const{
    return IsMouseButtonDown(button);
}
bool InputManager::GetMouseButtonUp(int button) const{
    return IsMouseButtonReleased(button);
}
bool InputManager::GetMouseState(int button) const{
    return IsMouseButtonPressed(button);
}