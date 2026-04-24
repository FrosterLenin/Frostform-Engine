#include "pong/Paddle.hpp"
#include "pong/PongGame.hpp"
#include "raylib.h"

Paddle::Paddle(Game* game, FVector2 position, FVector2 size, Color color, bool isAutomatic, int index) : 
    GameObject(game, position, size, color) 
    , _IsAutomatic(isAutomatic)
    , _RandomIndexVelocityX(1)
    , _Index(index){
    _Collider = std::make_shared<RectangleCollider>(position, size);
}
    
void Paddle::Draw(){
    GameObject::Draw();
    DrawRectangle(_Position.x, _Position.y, _Size.x, _Size.y, _Color);
}

void Paddle::Start(){
    _UP = _Index == 0 ? InputAction::MOVE_UP : InputAction::MOVE_UP_2;
    _DOWN = _Index == 0 ? InputAction::MOVE_DOWN : InputAction::MOVE_DOWN_2;
    _LEFT = _Index == 0 ? InputAction::MOVE_LEFT : InputAction::MOVE_LEFT_2;
    _RIGHT = _Index == 0 ? InputAction::MOVE_RIGHT : InputAction::MOVE_RIGHT_2;

    _Velocity = {1.0f, .0f};
    if(_IsAutomatic){
        // do while to avoid 0
        float rx, ry;
        do { rx = (float)GetRandomValue(-_RandomIndexVelocityX, _RandomIndexVelocityX); } while (rx == 0);
        _Velocity = (FVector2{rx, .0f}).Normalized();
    }
    _AccelerationIndex = 100.0f;
}
Paddle::~Paddle(){}

void Paddle::Update(float deltaTime){
    if(!_Game) return;
    if(!_IsAutomatic)
        UpdateWithInput(deltaTime);
    else{
        // Needed to check before clamping into bounding borders fo screen
        FVector2 nextPosition = GetCenter() + _Velocity.Normalized() * _AccelerationIndex * deltaTime;
        if(nextPosition.y - GetHalfHeight() < 0 || nextPosition.y + GetHalfHeight() > GetScreenHeight()){
            _Velocity.y *= -1;
        }
        if(nextPosition.x - GetHalfWidth() < 0 || nextPosition.x + GetHalfWidth() > GetScreenWidth()){
            _Velocity.x *= -1;
        }
    }
    SetPosition(_Position + _Velocity.Normalized() * _AccelerationIndex * deltaTime);
}
void Paddle::UpdateWithInput(float deltaTime){
    const InputManager* inputManager = dynamic_cast<PongGame*>(_Game)->GetInputManager();
    _Velocity = {0,0}; // Button released so it stops
    if(inputManager->GetActionState(_UP)){
        _Velocity.y = -1;
    }
    else if(inputManager->GetActionState(_DOWN)){
        _Velocity.y = 1;
    }
    if(inputManager->GetActionState(_RIGHT)){
        _Velocity.x = 1;
    }
    else if(inputManager->GetActionState(_LEFT)){
        _Velocity.x = -1;
    }
}
void Paddle::SetRandomIndexVelocityX(const int value){
    _RandomIndexVelocityX = value;
}

// NOT USED, MANAGED BY COLLISION MANAGER AND OnCollisionEnter/Exit/Stay METHODS
// AABB (Axis Aligned Bounding Box) the box in which resides the object is aligned to the axis of the window
// For collision to verify the two objects projections on the axis should overlap NOT only one
bool Paddle::CheckCollision(const GameObject& other) const{

    // Circle x rectangle collision
    // Closest point to the rectangle < circle radius = collision
    // Max(Box.X, Min(CircleCenter.x, Box.x + Widht)) < Circle.Radius = collision
    bool overlapX = _Position.x < other.GetPosition().x < _Position.x + _Size.x;
                    // || _Position.x < other.GetPosition().x + other.GetSize().x < _Position.x + _Size.x;
    bool overlapY = _Position.y < other.GetPosition().y < _Position.y + _Size.y;
                    // || _Position.y < other.GetPosition().y + other.GetSize().y < _Position.y + _Size.y;
    return overlapX && overlapY;
}

