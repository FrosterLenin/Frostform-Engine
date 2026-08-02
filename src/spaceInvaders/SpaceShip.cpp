#include "spaceInvaders/SpaceShip.hpp"
#include "spaceInvaders/SpaceInvaders.hpp"
#include "spaceInvaders/Invader.hpp"
#include "core/rasterizer/ACamera.hpp"
#include "core/StatBar.hpp"

SpaceShip::SpaceShip(Game* game, FVector2 position, FVector2 size, Color color, float accelerationIndex) : 
    Player(game, position, size, color, accelerationIndex) {
    // Player should collide with enemies and enemy projectiles, but not with other players or player projectiles
    _Collider = std::make_shared<RectangleCollider>(position, size, CollisionLayer::PLAYER, CollisionLayer::ENEMY | CollisionLayer::PROJECTILE_2); 
}
    
void SpaceShip::Draw(){
    if (ACamera* camera = GetCamera()) {
        Rectangle projectedRectangle = camera->ProjectRectangleTopLeft(_Position, _Size);
        DrawRectangle(
            static_cast<int>(projectedRectangle.x),
            static_cast<int>(projectedRectangle.y),
            static_cast<int>(projectedRectangle.width),
            static_cast<int>(projectedRectangle.height),
            _Color
        );
        return;
    }

    DrawRectangle(
        static_cast<int>(_Position.x),
        static_cast<int>(_Position.y),
        static_cast<int>(_Size.x),
        static_cast<int>(_Size.y),
        _Color
    );
}

void SpaceShip::Start(){
    _UP = InputAction::MOVE_UP;
    _DOWN = InputAction::MOVE_DOWN;
    _LEFT = InputAction::MOVE_LEFT;
    _RIGHT = InputAction::MOVE_RIGHT;
    _SHOOT = InputAction::SHOOT;

    _Velocity = {.0f, .0f};
    _AccelerationIndex = 100.0f;
    _Life = 100;

    SpaceInvaders* spaceInvadersGame = dynamic_cast<SpaceInvaders*>(_Game);
    // Initialize bullet pool
    for(int i = 0; i < MAX_BULLETS; ++i) {
        std::shared_ptr<Bullet> bullet = spaceInvadersGame->SpawnBullet(this, FVector2{-100.f, -100.f}, 5.f, BLUE, 200.f);
        bullet->OnExpired = [weakThis = weak_from_this()](std::shared_ptr<Bullet> inBullet)
        {
            if(std::shared_ptr<SpaceShip> sharedThis = weakThis.lock())
                sharedThis->ReturnBullet(inBullet);
        };
        _Bullets.push(bullet);
    }
}
SpaceShip::~SpaceShip(){}

void SpaceShip::Update(float deltaTime){
    if(!_Game) return;
    UpdateWithInput(deltaTime);
    SetPosition(_Position + _Velocity.Normalized() * _AccelerationIndex * deltaTime);
}
void SpaceShip::UpdateWithInput(float deltaTime){
    const InputManager* inputManager = dynamic_cast<SpaceInvaders*>(_Game)->GetInputManager();
    _Velocity = {0,0}; // Button released so it stops
    
    // Horizontal-only movement: vertical inputs are intentionally ignored
    // if(inputManager->GetActionState(_UP)){
    //     _Velocity.y = -1;
    // }
    // else if(inputManager->GetActionState(_DOWN)){
    //     _Velocity.y = 1;
    // }
    if(inputManager->GetActionState(_RIGHT)){
        _Velocity.x = 1;
    }
    else if(inputManager->GetActionState(_LEFT)){
        _Velocity.x = -1;
    }
    if(inputManager->GetActionDown(_SHOOT)){
        Shoot();
    }
}
void SpaceShip::Shoot(){
    if(_Bullets.empty()) return; // No bullets available in the pool
    auto bullet = _Bullets.front();
    if(bullet && !bullet->IsActive()){
        // Position the bullet in front of the spaceship (above it)
        // Center horizontally, and place it above the spaceship with some clearance
        float bulletRadius = bullet->GetRadius();
        float clearance = .1f; // Pixels of space between spaceship and bullet
        FVector2 bulletPosition = {
            GetCenter().x - bulletRadius  // Horizontal center of spaceship
            , _Position.y - (bulletRadius * 2.f) - clearance  // Above spaceship top
        };
        bullet->SetPosition(bulletPosition);
        bullet->SetActive(true);

        // Remove from the pool until it becomes inactive again (from hitting an enemy or going off screen)
        _Bullets.pop();
    }
}
void SpaceShip::ReturnBullet(std::shared_ptr<Bullet> bullet){
    bullet->SetActive(false);
    _Bullets.push(bullet);
}
void SpaceShip::OnCollisionEnter(FCollisionInfo& collisionInfo){
    GameObject::OnCollisionEnter(collisionInfo);

    if(dynamic_cast<Bullet*>(collisionInfo.OtherObject) != nullptr)
        TakeDamage(Bullet::DAMAGE);
    else if(dynamic_cast<Invader*>(collisionInfo.OtherObject) != nullptr)
        TakeDamage(Invader::COLLISION_DAMAGE);

    if(_Life <= 0){
        SetActive(false);
        _Game->SetShouldClose(true); // End the game loop, this will close the game after the current frame
    }
        
}
int SpaceShip::GetLife() const {
    return _Life;
}

void SpaceShip::TakeDamage(const int amount){
    _Life -= amount;
    
    // Trigger life bar update event through UIManager
    _Game->GetUIManager()->TriggerObjectEvent<StatBar>(_Life);
}