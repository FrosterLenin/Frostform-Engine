#include "pong/Ball.hpp"
#include "pong/PongGame.hpp"
#include "raylib.h"

Ball::Ball(Game* game, FVector2 position, float radius, Color color, bool isAutomatic, int index) : 
    Circle(game, position, radius, color) 
    , _IsAutomatic(isAutomatic)
    , _RandomIndexVelocityX(1)
    , _RandomIndexVelocityY(1)
    , _Index(index){
    _Collider = std::make_shared<CircleCollider>(position, radius);
}
void Ball::Start(){
    _UP = _Index == 0 ? InputAction::MOVE_UP : InputAction::MOVE_UP_2;
    _DOWN = _Index == 0 ? InputAction::MOVE_DOWN : InputAction::MOVE_DOWN_2;
    _LEFT = _Index == 0 ? InputAction::MOVE_LEFT : InputAction::MOVE_LEFT_2;
    _RIGHT = _Index == 0 ? InputAction::MOVE_RIGHT : InputAction::MOVE_RIGHT_2;

    _Velocity = {1.0f, 1.0f};
    if(_IsAutomatic){
        // do while to avoid 0
        float rx, ry;
        do { rx = (float)GetRandomValue(-_RandomIndexVelocityX, _RandomIndexVelocityX); } while (rx == 0);
        do { ry = (float)GetRandomValue(-_RandomIndexVelocityY, _RandomIndexVelocityY); } while (ry == 0);
        _Velocity = (FVector2{rx, ry}).Normalized();
    }
    _AccelerationIndex = 100.0f;
}
Ball::~Ball(){}

void Ball::Update(float deltaTime){
    if(!_Game) return;
    const InputManager* inputManager = dynamic_cast<PongGame*>(_Game)->GetInputManager();
    if(!_IsAutomatic)
        UpdateWithInput(deltaTime);
    else{
        // Needed to check before clamping into bounding borders fo screen
        FVector2 nextPosition = _Position + _Velocity.Normalized() * _AccelerationIndex * deltaTime;
        if(nextPosition.y < 0 || nextPosition.y + _Size.y > GetScreenHeight()){
            _Velocity.y *= -1;
        }
        int playerIndex = -1;
        if(nextPosition.x < 0){
            playerIndex = 1;
            _Velocity.x *= -1;
        }
        else if(nextPosition.x + _Size.x > GetScreenWidth()){
            playerIndex = 0;
            _Velocity.x *= -1;
        }
        if(playerIndex != -1){
            dynamic_cast<PongGame*>(_Game)->ScorePoint(playerIndex);
            SetPosition(_Game->GetScreenSize() * .5f);
            _AccelerationIndex = 100.0f;
        }
            
    }
    SetPosition(_Position + _Velocity.Normalized() * _AccelerationIndex * deltaTime);
}
void Ball::UpdateWithInput(float deltaTime){
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
void Ball::SetRandomIndexVelocityX(const int value){
    _RandomIndexVelocityX = value;
}
void Ball::SetRandomIndexVelocityY(const int value){
    _RandomIndexVelocityY = value;
}
void Ball::SetSpeedCap(const int value){
    _SpeedCap = value;
}
void Ball::OnCollisionEnter(FCollisionInfo& collisionInfo){
    GameObject::OnCollisionEnter(collisionInfo);
    if(collisionInfo.OverlapPoint.x < collisionInfo.OverlapPoint.y)
        _Velocity.x *= -1;
    else
        _Velocity.y *= -1;
    if(_AccelerationIndex < _SpeedCap)
        _AccelerationIndex *= 1.1f;
}

