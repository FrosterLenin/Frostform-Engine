#include "pong/PongGame.hpp"
#include "pong/Ball.hpp"
#include "pong/Paddle.hpp"
#include "core/ScoreUI.hpp"

PongGame::PongGame(FVector2 screenSize) : Game(screenSize, "Pong"), _Player1(nullptr), _Player2(nullptr){
    FVector2 centerScreen = screenSize * .5f;
    std::unique_ptr<Ball> ball = std::make_unique<Ball>(this, FVector2{centerScreen.x, centerScreen.y}, 5.0f, RAYWHITE, true);
    ball.get()->SetRandomIndexVelocityX(5);
    ball.get()->SetSpeedCap(400);
    _GameObjects.push_back(std::move(ball));
    
    std::unique_ptr<Paddle> paddle1 = std::make_unique<Paddle>(this, FVector2{100.0f, centerScreen.y}, FVector2{10.0f, 30.0f}, RAYWHITE, false);
    Paddle* paddle1Pointer = paddle1.get();
    _GameObjects.push_back(std::move(paddle1));
    _Player1 = paddle1Pointer;
    
    std::unique_ptr<Paddle> paddle2 = std::make_unique<Paddle>(this, FVector2{screenSize.x - 100.0f, centerScreen.y}, FVector2{10.0f, 30.0f}, RED, false, 1);
    Paddle* paddle2Pointer = paddle2.get();
    _GameObjects.push_back(std::move(paddle2));
    _Player2 = paddle2Pointer;
    
    InitUI();
    _GameOver = false;
    _EndGameDelay = .0f;
}

void PongGame::Update(float deltaTime){
    if(_GameOver){
        _EndGameDelay += deltaTime;
        // After 5 seconds of delay, the game will quit
        if(_EndGameDelay >= 5.0f){
            _ShouldClose = true;
            _GameOver = false;
        }
    }
    else{
        Game::Update(deltaTime);
        _UIManager->Update(deltaTime);
    }
}
        
void PongGame::Draw(){
    if(_GameOver)
        DrawEndGameScreen();
    else
        Game::Draw();
        
}
void PongGame::InitGame(const Color clearColor){
    _InputManager->BindKey(InputAction::MOVE_UP, KEY_W);
    _InputManager->BindKey(InputAction::MOVE_DOWN, KEY_S);
    _InputManager->BindKey(InputAction::MOVE_LEFT, KEY_A);
    _InputManager->BindKey(InputAction::MOVE_RIGHT, KEY_D);

    _InputManager->BindKey(InputAction::MOVE_UP_2, KEY_UP);
    _InputManager->BindKey(InputAction::MOVE_DOWN_2, KEY_DOWN);
    _InputManager->BindKey(InputAction::MOVE_LEFT_2, KEY_LEFT);
    _InputManager->BindKey(InputAction::MOVE_RIGHT_2, KEY_RIGHT);

    // Reset player scores
    if(_Player1) _Player1->ResetScore();
    if(_Player2) _Player2->ResetScore();

    Game::InitGame(clearColor);

}

int PongGame::GetPlayerScore(const int playerIndex) const{
    if(playerIndex == 0 && _Player1)
        return _Player1->GetScore();
    else if(playerIndex == 1 && _Player2)
        return _Player2->GetScore();
    return 0;
}

void PongGame::ScorePoint(const int playerIndex, const int score){
    _UIManager->GetManagedObject<ScoreUI>()->UpdateScore(playerIndex, score);
    int p1Score = _Player1 ? _Player1->GetScore() : 0;
    int p2Score = _Player2 ? _Player2->GetScore() : 0;

    if(p1Score >= 10 || p2Score >= 10)
        _GameOver = true;
}
void PongGame::DrawEndGameScreen(){
    BeginDrawing();
    ClearBackground(_ClearColor);
    const char* gameOverText = "Game Over";
    
    int p1Score = _Player1 ? _Player1->GetScore() : 0;
    int p2Score = _Player2 ? _Player2->GetScore() : 0;
    
    const char* winnerText = p1Score >= p2Score ? "Player 1 Wins!" : "Player 2 Wins!";
    std::string scoreTextString = std::to_string(p1Score) + " - " + std::to_string(p2Score);
    const char* scoreText = scoreTextString.c_str();
    Font font = GetFontDefault();    
    FVector2 centerScreen = _ScreenSize * .5f;
    
    Vector2 gameOverTextSize = MeasureTextEx(font, gameOverText, 20, 2);
    Vector2 winnerTextSize = MeasureTextEx(font, winnerText, 20, 2);
    Vector2 scoreTextSize = MeasureTextEx(font, scoreText, 20, 2);

    Vector2 gameOverTextOrigin = { gameOverTextSize.x * .5f, gameOverTextSize.y * .5f };
    Vector2 winnerTextOrigin = { winnerTextSize.x * .5f, winnerTextSize.y * .5f };
    Vector2 scoreTextOrigin = { scoreTextSize.x * .5f, scoreTextSize.y * .5f };

    DrawTextPro(font, gameOverText, centerScreen, gameOverTextOrigin, 0, 20, 2, RAYWHITE);
    DrawTextPro(font, winnerText, FVector2(centerScreen.x, centerScreen.y + 30), winnerTextOrigin, 0, 20, 2, RAYWHITE);
    DrawTextPro(font, scoreText, FVector2(centerScreen.x, centerScreen.y + 60), scoreTextOrigin, 0, 20, 2, RAYWHITE);

    EndDrawing();
}

void PongGame::InitUI(){
    _GameObjects.push_back(std::make_unique<ScoreUI>(this, 2 ,FVector2{10.0f, 10.0f}, FVector2{10.0f, 10.0f}, WHITE));
    ScoreUI* scoreUI = dynamic_cast<ScoreUI*>(_GameObjects.back().get());
    _UIManager->Bind(_GameObjects.back());
    if(scoreUI)
        _UIManager->BindEvent(scoreUI, scoreUI->OnScoreUpdated = [this](int playerIndex, int points){
            ScorePoint(playerIndex, points);
        });
}

