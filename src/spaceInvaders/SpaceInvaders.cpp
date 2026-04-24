#include "spaceInvaders/SpaceInvaders.hpp"
#include "spaceInvaders/SpaceShip.hpp"
#include "spaceInvaders/Invader.hpp"
#include "core/Background.hpp"
#include <memory>

SpaceInvaders::SpaceInvaders() : Game({800, 600}, "Space Invaders")
{
    FVector2 centerScreen = GetScreenSize() * .5f;
    FVector2 spaceShipSpawnPoint = {centerScreen.x, centerScreen.y + 200.0f};
    _GameObjects.push_back(std::make_unique<Background>(this, FVector2{.0f, .0f}, BLACK));
    _GameObjects.push_back(std::make_unique<SpaceShip>(this, spaceShipSpawnPoint, FVector2{40.0f, 40.0f}, WHITE, 100.0f));
    InitEnemies(5, 10, 10.0f);

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

std::shared_ptr<Bullet> SpaceInvaders::SpawnBullet(GameObject* owner, FVector2 position, float radius, Color color, float accelerationIndex) {
    auto bullet = std::make_shared<Bullet>(this, owner, position, radius, color, accelerationIndex);
    return std::static_pointer_cast<Bullet>(SpawnGameObject(bullet));
}

void SpaceInvaders::Update(float deltaTime){
    Game::Update(deltaTime);
    if(_EnemyManager.get()->IsEmpty())
        _ShouldClose = true;
}