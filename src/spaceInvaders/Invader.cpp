#include "spaceInvaders/Invader.hpp"
#include "spaceInvaders/SpaceInvaders.hpp"
#include "core/Game.hpp"
#include "core/ScoreUI.hpp"
#include <memory>

float Invader::_TimeSinceLastShot = 0.0f;

Invader::Invader(Game* game, FVector2 position, FVector2 size, Color color, float accelerationIndex) : 
    GameObject(game, position, size, color, accelerationIndex) {
    // Enemy should collide with players and player projectiles, but not with other enemies or enemy projectiles
    _Collider = std::make_shared<RectangleCollider>(position, size, CollisionLayer::ENEMY, CollisionLayer::PLAYER | CollisionLayer::PROJECTILE); 
}
Invader::~Invader(){}
void Invader::Draw(){
    GameObject::Draw();
    DrawRectangle(_Position.x, _Position.y, _Size.x, _Size.y, _Color);
}
void Invader::Start(){
    _Velocity = {1.f, .0}; // Move only in X axis
    _AccelerationIndex = 80.0f;

    SpaceInvaders* spaceInvadersGame = dynamic_cast<SpaceInvaders*>(_Game);
    // Initialize bullet pool
    for(int i = 0; i < MAX_BULLETS; ++i) {
        std::shared_ptr<Bullet> bullet = spaceInvadersGame->SpawnBullet(this, FVector2{-100.f, -100.f}, 5.f, YELLOW, 200.f, false);
        bullet->OnExpired = [weakThis = weak_from_this()](std::shared_ptr<Bullet> inBullet)
        {
            if(std::shared_ptr<Invader> sharedThis = weakThis.lock())
                sharedThis->ReturnBullet(inBullet);
        };
        _Bullets.push(bullet);
    }
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

void Invader::UpdateControlled(float deltaTime){
    _Velocity *= -1;
    SetPosition(FVector2{_Position.x, _Position.y + _Size.y});
}

void Invader::OnCollisionEnter(FCollisionInfo& collisionInfo){
    GameObject::OnCollisionEnter(collisionInfo);
    _Game->GetUIManager()->TriggerObjectEvent<ScoreUI>(0, 10); // Give 10 points to player 0 for killing an invader
    SetActive(false);
}

void Invader::Shoot(){
    if(_Bullets.empty()) return; // No bullets available in the pool
    auto bullet = _Bullets.front();
    if(bullet && !bullet->IsActive()){
        // Position the bullet in front of the invader (below it)
        // Center horizontally, and place it below the invader with some clearance
        float bulletRadius = bullet->GetRadius();
        float clearance = .1f; // Pixels of space between invader and bullet
        FVector2 bulletPosition = {
            GetCenter().x - bulletRadius  // Horizontal center of invader
            , _Position.y + _Size.y + clearance  // Below invader bottom
        };
        bullet->SetPosition(bulletPosition);
        bullet->SetActive(true);

        // Remove from the pool until it becomes inactive again (from hitting a player or going off screen)
        _Bullets.pop();
    }
}
void Invader::ReturnBullet(std::shared_ptr<Bullet> bullet){
    bullet->SetActive(false);
    _Bullets.push(bullet);
}