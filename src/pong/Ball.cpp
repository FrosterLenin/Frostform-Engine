#include "pong/Ball.hpp"
#include "pong/PongGame.hpp"
#include "core/ScoreUI.hpp"
#include "raylib.h"

Ball::Ball(Game* game, FVector2 position, float radius, Color color, bool isAutomatic, int index) : 
    Circle(game, position, radius, color) 
    , _IsAutomatic(isAutomatic)
    , _RandomIndexVelocityX(1)
    , _RandomIndexVelocityY(1)
    , _Index(index)
    , _SpeedCap(0)
    , _HasSpeedCap(false){
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
    _AccelerationIndex = PongGame::BASE_ACCELERATION;
}
Ball::~Ball(){}

void Ball::Update(float deltaTime){
    if(!_Game) return;
    // [[maybe_unused]] tells the compiler this variable may be intentionally unused
    // Here we keep inputManager ready for quick debug experiments on ball trajectory/input
    // without triggering "unused variable" warnings in normal builds
    [[maybe_unused]] const InputManager* inputManager = dynamic_cast<PongGame*>(_Game)->GetInputManager();
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
            // Ball detected a score, call PongGame to trigger the ScoreUI event
            _Game->GetUIManager()->TriggerObjectEvent<ScoreUI>(playerIndex, 1);
            SetPosition(_Game->GetScreenSize() * .5f);
            _AccelerationIndex = PongGame::BASE_ACCELERATION; // Reset speed on score
            // Stop here so the ball does not move again in the same scoring frame.
            return;
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
    _HasSpeedCap = true;
}
void Ball::OnCollisionEnter(FCollisionInfo& collisionInfo){
    GameObject::OnCollisionEnter(collisionInfo);
    // OverlapPoint may come with signed values depending on collider pair order,
    // but for axis selection we only care about penetration magnitude per axis
    float overlapX = collisionInfo.OverlapPoint.x;
    float overlapY = collisionInfo.OverlapPoint.y;
    if (overlapX < 0.0f) overlapX = -overlapX;
    if (overlapY < 0.0f) overlapY = -overlapY;

    // Small tolerance to avoid unstable axis flips when overlaps are almost equal
    constexpr float tollerance = 0.0001f;
    if (overlapX + tollerance < overlapY) {
        // Less penetration on X means we likely touched a vertical face: reflect X
        _Velocity.x *= -1;
    } else if (overlapY + tollerance < overlapX) {
        // Less penetration on Y means we likely touched a horizontal face: reflect Y
        _Velocity.y *= -1;
    } else {
        // Corner/near-tie case: choose a deterministic axis from incoming velocity
        // This prevents random-looking bounces when both overlaps are almost identical
        float velocityX = _Velocity.x < 0.0f ? -_Velocity.x : _Velocity.x;
        float velocityY = _Velocity.y < 0.0f ? -_Velocity.y : _Velocity.y;
        if (velocityX >= velocityY)
            _Velocity.x *= -1;
        else
            _Velocity.y *= -1;
    }

    if(_HasSpeedCap && _SpeedCap > 0 && _AccelerationIndex < static_cast<float>(_SpeedCap)) {
        // Keep the existing acceleration feel while enforcing a strict upper limit
        _AccelerationIndex *= 1.1f;
        if (_AccelerationIndex > static_cast<float>(_SpeedCap))
            _AccelerationIndex = static_cast<float>(_SpeedCap);
    }
}

