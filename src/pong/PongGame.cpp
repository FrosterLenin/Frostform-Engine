#include "pong/PongGame.hpp"
#include "pong/Ball.hpp"
#include "pong/Paddle.hpp"
#include "pong/ScoreUI.hpp"

PongGame::PongGame(FVector2 screenSize) : Game(screenSize, "Pong"){
    _UIManager = std::make_unique<UIManager>();
    FVector2 centerScreen = screenSize * .5f;
    std::unique_ptr<Ball> ball = std::make_unique<Ball>(this, FVector2{centerScreen.x, centerScreen.y}, 5.0f, RAYWHITE, true);
    ball.get()->SetRandomIndexVelocityX(5);
    ball.get()->SetSpeedCap(400);
    _GameObjects.push_back(std::move(ball));
    _GameObjects.push_back(std::make_unique<Paddle>(this, FVector2{100.0f, centerScreen.y}, FVector2{10.0f, 30.0f}, RAYWHITE, false));
    _GameObjects.push_back(std::make_unique<Paddle>(this, FVector2{screenSize.x - 100.0f, centerScreen.y}, FVector2{10.0f, 30.0f}, RED, false, 1));
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

    _P1Points = 0;
    _P2Points = 0;

    Game::InitGame(clearColor);

}

int PongGame::GetPlayerPoints(const int playerIndex) const{
    return playerIndex == 0 ? _P1Points : _P2Points;
}
void PongGame::ScorePoint(const int playerIndex){
    if(playerIndex == 0)
        ++_P1Points;
    else
        ++_P2Points;
    
    // Trigger UIManager event with playerIndex
    _UIManager->TriggerAllEvents(playerIndex);

    if(_P1Points >= 10 || _P2Points >= 10){
        _GameOver = true;
    }
}
void PongGame::DrawEndGameScreen(){
    BeginDrawing();
    ClearBackground(_ClearColor);
    const char* gameOverText = "Game Over";
    const char* winnerText = _P1Points >= _P2Points ? "Player 1 Wins!" : "Player 2 Wins!";
    std::string pointsTextString = std::to_string(_P1Points) + " - " + std::to_string(_P2Points);
    const char* pointsText = pointsTextString.c_str();
    Font font = GetFontDefault();    
    FVector2 centerScreen = _ScreenSize * .5f;
    
    Vector2 gameOverTextSize = MeasureTextEx(font, gameOverText, 20, 2);
    Vector2 winnerTextSize = MeasureTextEx(font, winnerText, 20, 2);
    Vector2 pointsTextSize = MeasureTextEx(font, pointsText, 20, 2);

    Vector2 gameOverTextOrigin = { gameOverTextSize.x * .5f, gameOverTextSize.y * .5f };
    Vector2 winnerTextOrigin = { winnerTextSize.x * .5f, winnerTextSize.y * .5f };
    Vector2 pointsTextOrigin = { pointsTextSize.x * .5f, pointsTextSize.y * .5f };

    DrawTextPro(font, gameOverText, centerScreen, gameOverTextOrigin, 0, 20, 2, RAYWHITE);
    DrawTextPro(font, winnerText, FVector2(centerScreen.x, centerScreen.y + 30), winnerTextOrigin, 0, 20, 2, RAYWHITE);
    DrawTextPro(font, pointsText, FVector2(centerScreen.x, centerScreen.y + 60), pointsTextOrigin, 0, 20, 2, RAYWHITE);

    EndDrawing();
}

void PongGame::InitUI(){
    _GameObjects.push_back(std::make_unique<ScoreUI>(this, FVector2{10.0f, 10.0f}, FVector2{10.0f, 10.0f}, WHITE));
    ScoreUI* scoreUI = dynamic_cast<ScoreUI*>(_GameObjects.back().get());
    _UIManager->Bind(_GameObjects.back());
    
    if(scoreUI) {
        _UIManager->BindEvent(scoreUI, scoreUI->ScoreEvent);
    }
}

