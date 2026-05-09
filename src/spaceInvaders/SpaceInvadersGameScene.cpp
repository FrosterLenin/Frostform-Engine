#include "spaceInvaders/SpaceInvadersGameScene.hpp"
#include "spaceInvaders/SpaceShip.hpp"
#include "spaceInvaders/Invader.hpp"
#include "spaceInvaders/Bullet.hpp"
#include "spaceInvaders/SpaceInvaders.hpp"
#include "core/Background.hpp"
#include "core/Game.hpp"
#include <memory>

SpaceInvadersGameScene::SpaceInvadersGameScene(Game* game)
    : Scene(game), _Player(nullptr), _ScoreUI(nullptr)
{
}

void SpaceInvadersGameScene::Init()
{
    _State = SceneState::ACTIVE;
    
    // Set this scene on the SpaceInvaders game instance immediately so bullets can be spawned
    SpaceInvaders* spaceInvadersGame = dynamic_cast<SpaceInvaders*>(_Game);
    if (spaceInvadersGame)
        spaceInvadersGame->SetGameScene(this);
    
    FVector2 centerScreen = _Game->GetScreenSize() * 0.5f;
    FVector2 spaceShipSpawnPoint = {centerScreen.x, centerScreen.y + 200.0f};
    
    // Spawn background in picture mode (clear color remains a fallback)
    SpawnGameObject<Background>(_Game, FVector2{0.0f, 0.0f}, "src/spaceInvaders/resources/space.jpg", BLACK);
    
    // Spawn player spaceship
    std::weak_ptr<SpaceShip> playerShip = SpawnGameObject<SpaceShip>(_Game, spaceShipSpawnPoint, 
                                                  FVector2{40.0f, 40.0f}, WHITE, 100.0f);
    _Player = playerShip.lock().get();
    
    // Initialize enemies
    InitEnemies(3, 7, 20.0f);
    
    // Setup input bindings
    _Game->GetInputManager()->BindKey(InputAction::MOVE_UP, KEY_W);
    _Game->GetInputManager()->BindKey(InputAction::MOVE_DOWN, KEY_S);
    _Game->GetInputManager()->BindKey(InputAction::MOVE_LEFT, KEY_A);
    _Game->GetInputManager()->BindKey(InputAction::MOVE_RIGHT, KEY_D);
    _Game->GetInputManager()->BindKey(InputAction::SHOOT, KEY_SPACE);
    
    // Initialize UI
    InitUI();
}

SceneResult SpaceInvadersGameScene::Update(float deltaTime)
{
    // Update input
    _Game->GetInputManager()->Update();
    
    // Update all game objects
    for (std::shared_ptr<GameObject>& gameObject : _GameObjects) {
        if (!gameObject->IsActive()) continue;
        gameObject->Update(deltaTime);
    }
    
    // Update managers
    _Game->GetCollisionManager()->Update();
    _Game->GetUIManager()->Update(deltaTime);
    // NOTE: EnemyManager::Update() is called by individual invaders when they detect edge collision
    // _Game->GetEnemyManager()->Update(deltaTime);  // Don't call this - it's handled in Invader::Update()
    
    // Tick invader shots
    TickInvaderShot(deltaTime);
    
    // Check if all enemies defeated
    if (_Game->GetEnemyManager()->IsEmpty())
        return SceneResult::EXIT;
    
    return SceneResult::CONTINUE;
}

void SpaceInvadersGameScene::Draw()
{
    BeginDrawing();
    ClearBackground(_Game->GetClearColor());
    _Game->GetDrawManager()->BeginDraw();
    EndDrawing();
}

void SpaceInvadersGameScene::Destroy()
{
    _State = SceneState::UNLOADING;
    _GameObjects.clear();
}

std::shared_ptr<Bullet> SpaceInvadersGameScene::SpawnBullet(GameObject* owner, FVector2 position, float radius, 
                                                             Color color, float accelerationIndex, bool isPlayer)
{
    FVector2 bulletVelocity = isPlayer ? FVector2{0.0f, -1.0f} : FVector2{0.0f, 1.0f}; // Move only in Y axis
    std::shared_ptr<Bullet> bullet = std::make_shared<Bullet>(_Game, owner, position, radius, color, 
                                                               accelerationIndex, bulletVelocity);
    if (isPlayer) {
        bullet->GetCollider()->_Layer = CollisionLayer::PROJECTILE;
        bullet->GetCollider()->_Mask = ~(CollisionLayer::PLAYER | CollisionLayer::PROJECTILE); 
    } else {
        bullet->GetCollider()->_Layer = CollisionLayer::PROJECTILE_2;
        bullet->GetCollider()->_Mask = ~(CollisionLayer::ENEMY | CollisionLayer::PROJECTILE_2);
    }
    
    std::shared_ptr<GameObject> result = SpawnGameObject(bullet);
    _Game->GetEnemyManager()->Bind(result); // Bind to enemy manager for tracking
    return std::static_pointer_cast<Bullet>(result);
}

void SpaceInvadersGameScene::InitEnemies(int row, int column, float spacing)
{
    FVector2 startPosition = {10.0f, 10.0f};
    FVector2 size = {40.0f, 40.0f};
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < column; ++j) {
            FVector2 position = startPosition + FVector2{j * (size.x + spacing), i * (size.y + spacing)};
            std::weak_ptr<Invader> invader = SpawnGameObject<Invader>(_Game, position, size, RED, 300.0f);
            _Game->GetEnemyManager()->Bind(invader.lock());
        }
    }
}

void SpaceInvadersGameScene::InitUI()
{
    std::weak_ptr<ScoreUI> scoreUI = SpawnGameObject<ScoreUI>(_Game, 1, FVector2{10.0f, 10.0f}, 
                                            FVector2{20.0f, 20.0f}, WHITE);
    _ScoreUI = scoreUI.lock().get();
    
    if (_ScoreUI) {
        // Register player with ScoreUI
        if (_Player)
            _ScoreUI->AddPlayer(_Player);
        
        _Game->GetUIManager()->Bind(scoreUI.lock());
        _Game->GetUIManager()->BindEvent(_ScoreUI, _ScoreUI->UpdateEvent = 
            [this](int playerIndex, int points) {
                ScorePoint(playerIndex, points);
            });
    }
}

void SpaceInvadersGameScene::ScorePoint(const int playerIndex, const int score)
{
    if (_ScoreUI)
        _ScoreUI->UpdateScore(playerIndex, score);
}

void SpaceInvadersGameScene::TickInvaderShot(float deltaTime)
{
    // Advance the shared timer (all invaders share the same shooting cadence)
    Invader::_TimeSinceLastShot += deltaTime;
    if (Invader::_TimeSinceLastShot < Invader::SHOOT_INTERVAL) return;

    const std::vector<std::weak_ptr<GameObject>> managed = _Game->GetEnemyManager()->GetManagedObjects();
    const size_t count = managed.size();
    if (count > 0) {
        // Start from a random index so the shooter varies each interval
        const size_t start = rand() % count;
        // Walk forward until an active invader is found, guaranteeing a shot if any are alive
        for (size_t i = 0; i < count; ++i) {
            std::shared_ptr<GameObject> managedObject = managed[(start + i) % count].lock();
            if (managedObject && managedObject->IsActive()) {
                if (Invader* invader = dynamic_cast<Invader*>(managedObject.get())) {
                    invader->Shoot();
                    break;
                }
            }
        }
    }

    Invader::_TimeSinceLastShot -= Invader::SHOOT_INTERVAL;
}
