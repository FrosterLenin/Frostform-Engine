#include "spaceInvaders/SpaceInvaders.hpp"
#include "spaceInvaders/SpaceShip.hpp"
#include "spaceInvaders/Invader.hpp"
#include "core/Background.hpp"
#include "core/ScoreUI.hpp"
#include <memory>

SpaceInvaders::SpaceInvaders() : Game({800, 600}, "Space Invaders")
{
    FVector2 centerScreen = GetScreenSize() * .5f;
    FVector2 spaceShipSpawnPoint = {centerScreen.x, centerScreen.y + 200.0f};
    _GameObjects.push_back(std::make_unique<Background>(this, FVector2{.0f, .0f}, BLACK));
    _GameObjects.push_back(std::make_unique<SpaceShip>(this, spaceShipSpawnPoint, FVector2{40.0f, 40.0f}, WHITE, 100.0f));
    InitEnemies(3, 10, 20.0f);
    InitUI();
}

void SpaceInvaders::InitGame(const Color clearColor){
    _InputManager->BindKey(InputAction::MOVE_UP, KEY_W);
    _InputManager->BindKey(InputAction::MOVE_DOWN, KEY_S);
    _InputManager->BindKey(InputAction::MOVE_LEFT, KEY_A);
    _InputManager->BindKey(InputAction::MOVE_RIGHT, KEY_D);
    _InputManager->BindKey(InputAction::SHOOT, KEY_SPACE);
    Game::InitGame(clearColor);
}

void SpaceInvaders::InitEnemies(int row, int column, float spacing){
    FVector2 startPosition = {10.0f, 10.0f};
    FVector2 size = {40.0f, 40.0f};
    for(int i = 0; i < row; ++i){
        for(int j = 0; j < column; ++j){
            FVector2 position = startPosition + FVector2{j * (size.x + spacing), i * (size.y + spacing)};
            _GameObjects.push_back(std::make_unique<Invader>(this, position, size, RED, 300.0f));
            _EnemyManager->Bind(_GameObjects.back());
        }
    }
}

std::shared_ptr<Bullet> SpaceInvaders::SpawnBullet(GameObject* owner, FVector2 position, float radius, Color color, float accelerationIndex, bool isPlayer) {
    FVector2 bulletVelocity = isPlayer ? FVector2{.0f, -1.0f} : FVector2{.0f, 1.0f}; // Move only in Y axis
    std::shared_ptr<Bullet> bullet = std::make_shared<Bullet>(this, owner, position, radius, color, accelerationIndex, bulletVelocity);
    if(isPlayer){
        bullet.get()->GetCollider()->_Layer = CollisionLayer::PROJECTILE;
        bullet.get()->GetCollider()->_Mask = ~(CollisionLayer::PLAYER | CollisionLayer::PROJECTILE); // Player bullets should not collide with other player bullets or with other players
    }
    else{
        bullet.get()->GetCollider()->_Layer = CollisionLayer::PROJECTILE_2;
        bullet.get()->GetCollider()->_Mask = ~(CollisionLayer::ENEMY | CollisionLayer::PROJECTILE_2); // Enemy bullets should not collide with other enemy bullets or with other enemies
    }
    return std::static_pointer_cast<Bullet>(SpawnGameObject(bullet));
}

void SpaceInvaders::Update(float deltaTime){
    Game::Update(deltaTime);

    TickInvaderShot(deltaTime);

    if(_EnemyManager.get()->IsEmpty())
        _ShouldClose = true;
}
void SpaceInvaders::TickInvaderShot(float deltaTime){
    // Advance the shared timer (all invaders share the same shooting cadence)
    Invader::_TimeSinceLastShot += deltaTime;
    if(Invader::_TimeSinceLastShot < Invader::SHOOT_INTERVAL) return;

    const auto& managed = _EnemyManager->GetManagedObjects();
    const size_t count = managed.size();
    if(count > 0){
        // Start from a random index so the shooter varies each interval
        const size_t start = rand() % count;
        // Walk forward until an active invader is found, guaranteeing a shot if any are alive
        for(size_t i = 0; i < count; ++i){
            std::shared_ptr<GameObject> managedObject = managed[(start + i) % count].lock();
            if(managedObject && managedObject->IsActive()){
                if(Invader* invader = dynamic_cast<Invader*>(managedObject.get())){
                    invader->Shoot();
                    break;
                }
            }
        }
    }

    Invader::_TimeSinceLastShot -= Invader::SHOOT_INTERVAL;
}

void SpaceInvaders::ScorePoint(const int playerIndex, const int score){
    _UIManager->GetManagedObject<ScoreUI>()->UpdateScore(playerIndex, score);

    // if(p1Score >= 10 || p2Score >= 10)
    //     _GameOver = true;
}
void SpaceInvaders::InitUI(){
    _GameObjects.push_back(std::make_unique<ScoreUI>(this, 1 ,FVector2{10.0f, 10.0f}, FVector2{10.0f, 10.0f}, WHITE));
    ScoreUI* scoreUI = dynamic_cast<ScoreUI*>(_GameObjects.back().get());
    _UIManager->Bind(_GameObjects.back());
    
    if(scoreUI)
        _UIManager->BindEvent(scoreUI, scoreUI->OnScoreUpdated = [this](int playerIndex, int points){
            ScorePoint(playerIndex, points);
    });
}