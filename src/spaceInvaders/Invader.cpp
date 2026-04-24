#include "spaceInvaders/Invader.hpp"
#include "core/Game.hpp"


Invader::Invader(Game* game, FVector2 position, FVector2 size, Color color, float accelerationIndex) : 
    GameObject(game, position, size, color, accelerationIndex) {
    _Collider = std::make_shared<RectangleCollider>(position, size);
    
}
Invader::~Invader(){}
void Invader::Draw(){
    GameObject::Draw();
    DrawRectangle(_Position.x, _Position.y, _Size.x, _Size.y, _Color);
}
void Invader::Start(){
    _Velocity = {1.f, .0}; // Move only in X axis
    _AccelerationIndex = 100.0f;
}

void Invader::Update(float deltaTime){
    if(!_Game) return;
    
    // Needed to check before clamping into bounding borders fo screen
    FVector2 nextPosition = GetCenter() + _Velocity * _AccelerationIndex * deltaTime;
    // We have no Y movement so we dont need to check for it
    // if(nextPosition.y - GetHalfHeight() < 0 || nextPosition.y + GetHalfHeight() > GetScreenHeight()){
    //     _Velocity.y *= -1;
    // }
    if(nextPosition.x - GetHalfWidth() < 0 || nextPosition.x + GetHalfWidth() > GetScreenWidth()){
        EnemyManager* enemyManager = const_cast<EnemyManager*>(_Game->GetEnemyManager());
        enemyManager->Update(deltaTime);
        return; // Don't move again this frame, the formation already moved
    }
    SetPosition(_Position + _Velocity * _AccelerationIndex * deltaTime);
}

void Invader::UpdateAutomatic(float deltaTime){
    _Velocity *= -1;
    SetPosition(FVector2{_Position.x, _Position.y + _Size.y});
}

void Invader::OnCollisionEnter(FCollisionInfo& collisionInfo){
    GameObject::OnCollisionEnter(collisionInfo);
    SetActive(false);
}