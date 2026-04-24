#include "spaceInvaders/SpaceShip.hpp"
#include "spaceInvaders/SpaceInvaders.hpp"

SpaceShip::SpaceShip(Game* game, FVector2 position, FVector2 size, Color color, float accelerationIndex) : 
    GameObject(game, position, size, color, accelerationIndex) {
    _Collider = std::make_shared<RectangleCollider>(position, size);
}
    
void SpaceShip::Draw(){
    GameObject::Draw();
    DrawRectangle(_Position.x, _Position.y, _Size.x, _Size.y, _Color);
}

void SpaceShip::Start(){
    _UP = InputAction::MOVE_UP;
    _DOWN = InputAction::MOVE_DOWN;
    _LEFT = InputAction::MOVE_LEFT;
    _RIGHT = InputAction::MOVE_RIGHT;
    _SHOOT = InputAction::SHOOT;

    _Velocity = {.0f, .0f};
    _AccelerationIndex = 100.0f;

    SpaceInvaders* spaceInvadersGame = dynamic_cast<SpaceInvaders*>(_Game);
    // Initialize bullet pool
    for(int i = 0; i < 2; ++i) {
        std::shared_ptr<Bullet> bullet = spaceInvadersGame->SpawnBullet(this, FVector2{-100.f, -100.f}, 10.f, BLUE, 200.f);
        bullet->OnExpired = [weakThis = weak_from_this()](std::shared_ptr<Bullet> inBullet)
        {
            if(std::shared_ptr<SpaceShip> sharedThis = weakThis.lock()){
                sharedThis->ReturnBullet(inBullet);
            }
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