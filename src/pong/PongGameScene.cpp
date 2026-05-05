#include "pong/PongGameScene.hpp"
#include "pong/PongGame.hpp"
#include "pong/Ball.hpp"
#include "pong/Paddle.hpp"
#include "core/Game.hpp"

PongGameScene::PongGameScene(Game* game) 
    : Scene(game), _Player1(nullptr), _Player2(nullptr), _Ball(nullptr), _ScoreUI(nullptr)
{
}

void PongGameScene::Init()
{
    _State = SceneState::ACTIVE;
    
    // Set this scene on the PongGame instance immediately
    PongGame* pongGame = dynamic_cast<PongGame*>(_Game);
    if (pongGame)
        pongGame->SetGameScene(this);
    
    FVector2 centerScreen = _Game->GetScreenSize() * 0.5f;
    
    // Spawn ball
    std::weak_ptr<Ball> ball = SpawnGameObject<Ball>(_Game, centerScreen, 5.0f, RAYWHITE, true);
    _Ball = ball.lock().get();
    if (_Ball) {
        _Ball->SetRandomIndexVelocityX(5);
        _Ball->SetSpeedCap(400);
    }
    
    // Spawn player 1 paddle
    std::weak_ptr<Paddle> paddle1 = SpawnGameObject<Paddle>(_Game, FVector2{100.0f, centerScreen.y}, 
                                           FVector2{10.0f, 30.0f}, RAYWHITE, false);
    _Player1 = paddle1.lock().get();
    if (_Player1)
        _Player1->ResetScore();
    
    // Spawn player 2 paddle
    std::weak_ptr<Paddle> paddle2 = SpawnGameObject<Paddle>(_Game, 
                                           FVector2{_Game->GetScreenSize().x - 100.0f, centerScreen.y}, 
                                           FVector2{10.0f, 30.0f}, RED, false, 1);
    _Player2 = paddle2.lock().get();
    if (_Player2)
        _Player2->ResetScore();
    
    // Setup input bindings
    _Game->GetInputManager()->BindKey(InputAction::MOVE_UP, KEY_W);
    _Game->GetInputManager()->BindKey(InputAction::MOVE_DOWN, KEY_S);
    _Game->GetInputManager()->BindKey(InputAction::MOVE_UP_2, KEY_UP);
    _Game->GetInputManager()->BindKey(InputAction::MOVE_DOWN_2, KEY_DOWN);
    
    // Initialize UI
    InitUI();
}

SceneResult PongGameScene::Update(float deltaTime)
{
    // Update input
    _Game->GetInputManager()->Update();
    
    // Update all game objects
    for (std::shared_ptr<GameObject>& gameObject : _GameObjects) {
        if (!gameObject->IsActive()) continue;
        gameObject->Update(deltaTime);
    }
    
    // Update collision and UI
    _Game->GetCollisionManager()->Update();
    _Game->GetUIManager()->Update(deltaTime);
    
    // Check for game over
    int p1Score = _Player1 ? _Player1->GetScore() : 0;
    int p2Score = _Player2 ? _Player2->GetScore() : 0;
    
    if (p1Score >= WINNING_SCORE || p2Score >= WINNING_SCORE)
        return SceneResult::SCENE_COMPLETE;
    
    return SceneResult::CONTINUE;
}

void PongGameScene::Draw()
{
    BeginDrawing();
    ClearBackground(_Game->GetClearColor());
    _Game->GetDrawManager()->BeginDraw();
    EndDrawing();
}

void PongGameScene::Destroy()
{
    _State = SceneState::UNLOADING;
    _GameObjects.clear();
}

int PongGameScene::GetPlayerScore(const int playerIndex) const
{
    if (playerIndex == 0 && _Player1)
        return _Player1->GetScore();
    else if (playerIndex == 1 && _Player2)
        return _Player2->GetScore();
    return 0;
}

void PongGameScene::ScorePoint(const int playerIndex, const int score)
{
    if (_ScoreUI)
        _ScoreUI->UpdateScore(playerIndex, score);
}

void PongGameScene::InitUI()
{
    std::weak_ptr<ScoreUI> scoreUI = SpawnGameObject<ScoreUI>(_Game, 2, FVector2{10.0f, 10.0f}, 
                                            FVector2{10.0f, 10.0f}, WHITE);
    _ScoreUI = scoreUI.lock().get();
    
    if (_ScoreUI) {
        // Register players with ScoreUI
        if (_Player1)
            _ScoreUI->AddPlayer(_Player1);
        if (_Player2)
            _ScoreUI->AddPlayer(_Player2);
        
        _Game->GetUIManager()->Bind(scoreUI.lock());
        _Game->GetUIManager()->BindEvent(_ScoreUI, _ScoreUI->UpdateEvent = 
            [this](int playerIndex, int points) {
                ScorePoint(playerIndex, points);
            });
    }
}
