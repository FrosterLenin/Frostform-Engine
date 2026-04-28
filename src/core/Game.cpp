#include "core/Game.hpp"
#include "core/Player.hpp"

Game::Game(FVector2 screenSize, const std::string& title) : 
    _ScreenSize(screenSize)
    , _Title(title)
    , _ShouldClose(false)
    , _ClearColor(RAYWHITE){
    InitWindow(_ScreenSize.x, _ScreenSize.y, _Title.c_str());
    SetTargetFPS(60);
    _InputManager = std::make_unique<InputManager>();
    _CollisionManager = std::make_unique<CollisionManager>();
    _DrawManager = std::make_unique<DrawManager>();
    _EnemyManager = std::make_unique<EnemyManager>();
    _UIManager = std::make_unique<UIManager>();
}
Game::~Game(){
    CloseWindow();
}
const FVector2 Game::GetScreenSize() const{
    return _ScreenSize;
}
const InputManager* Game::GetInputManager() const{
    return _InputManager.get();
}
const EnemyManager* Game::GetEnemyManager() const{
    return _EnemyManager.get();
}
const UIManager* Game::GetUIManager() const{
    return _UIManager.get();
}
const std::vector<Player*> Game::GetPlayers() const{
    std::vector<Player*> players;
    for(const auto& gameObject : _GameObjects){
        Player* player = dynamic_cast<Player*>(gameObject.get());
        if(player != nullptr){
            players.push_back(player);
        }
    }
    return players;
}
void Game::SetShouldClose(const bool shouldClose){
    _ShouldClose = shouldClose;
}
void Game::SetClearColor(const Color other){
    _ClearColor = other;
}
void Game::Draw(){
    BeginDrawing();
    ClearBackground(_ClearColor);
    _DrawManager.get()->BeginDraw();
    EndDrawing();
}
void Game::Update(float deltaTime){
    _InputManager.get()->Update();
    for(auto& gameObject : _GameObjects){
        if(!gameObject->IsActive()) continue;
        gameObject->Update(deltaTime);
    }
    _CollisionManager.get()->Update();
}
void Game::Run(){
    while(!WindowShouldClose() && !_ShouldClose){
        float deltaTime = GetFrameTime();
        Update(deltaTime);
        Draw();
    }
}
void Game::Quit(){
    _ShouldClose = true;
}

void Game::InitGame(const Color clearColor){
    _DrawManager.get()->Init();
    SetClearColor(clearColor);
     for(auto& gameObject : _GameObjects){
        if(!gameObject->IsActive()) continue;
        gameObject->Start();
        _CollisionManager.get()->RegisterCollider(gameObject);
        _DrawManager.get()->BindObject(gameObject);
    }
}

std::shared_ptr<GameObject> Game::SpawnGameObject(std::shared_ptr<GameObject> gameObject) {
    gameObject->SetActive(false);
    gameObject->Start();

    _GameObjects.push_back(gameObject);
    _CollisionManager->RegisterCollider(gameObject);
    _DrawManager->BindObject(gameObject);

    return gameObject;
}