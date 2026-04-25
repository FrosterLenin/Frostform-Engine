#include "core/GameObject.hpp"
#include "core/Game.hpp"
#include "raylib.h"
#include <algorithm>

GameObject::GameObject(Game* game, const FVector2 position, const FVector2 size) :
    _Game(game)
    , _Position(position)
    , _Velocity({0,0})
    , _AccelerationIndex(.0f)
    , _Size(size)
    , _Active(true)
    , _DrawLayer(DrawLayer::GAME_FIELD){
}
GameObject::GameObject(Game* game, const FVector2 position, const FVector2 size, const Color color, float accelerationIndex) :
    GameObject(game, position, size, color){
    _AccelerationIndex = accelerationIndex;
}
GameObject::GameObject(Game* game, const FVector2 position, const FVector2 size, const Color color) :
    GameObject(game, position, size)
{
    _Color = color;
}
GameObject::GameObject(Game* game, const FVector2 position, const Color color) :
    GameObject(game, position)
{
    _Color = color;
}
GameObject::~GameObject() {}

void GameObject::SetPosition(const FVector2& other){
    if (!_Game) {
        _Position = other;
        return;
    }
    FVector2 screenSize = _Game->GetScreenSize();

    // Clamp in screensize considering object size
    _Position.x = std::clamp(other.x, .0f, screenSize.x - _Size.x);
    _Position.y = std::clamp(other.y, .0f, screenSize.y - _Size.y);

    if(GetCollider()){
        _Collider.get()->_Position = _Position;
    }
}
void GameObject::OnCollisionEnter(FCollisionInfo& collisionInfo){

}
void GameObject::OnCollisionExit(FCollisionInfo& collisionInfo){

}
void GameObject::OnCollisionStay(FCollisionInfo& collisionInfo){

}
void GameObject::SetVelocity(const FVector2& other){
    _Velocity = other;
}
void GameObject::SetActive(const bool other){
    _Active = other;
}
FVector2 GameObject::GetPosition() const{
    return _Position;
}
FVector2 GameObject::GetVelocity() const{
    return _Velocity;
}
FVector2 GameObject::GetSize() const{
    return _Size;
}
Collider* GameObject::GetCollider() const{
    return _Collider.get();
}
bool GameObject::IsActive() const{
    return _Active;
}
void GameObject::Destroy(){
    _Active = false;
}
float GameObject::GetHalfWidth() const{
    return _Size.x * .5f;
}
float GameObject::GetHalfHeight() const{
    return _Size.y * .5f;
}
FVector2 GameObject::GetCenter() const{
    return FVector2{ _Position.x + GetHalfWidth(), _Position.y + GetHalfHeight() };
}
DrawLayer GameObject::GetDrawLayer() const{
    return _DrawLayer;
}
void GameObject::DrawCenter() const{
    FVector2 center = GetCenter();
    DrawCircle((int)center.x, (int)center.y, 1.f, GREEN);
}
void GameObject::Draw() {

}
void GameObject::UpdateWithInput(float deltaTime){
}
void GameObject::UpdateControlled(float deltaTime){
}
void GameObject::Update(float deltaTime){
}

bool GameObject::CheckCollision(const GameObject& other) const{
    return false;
}
