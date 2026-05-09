#include "pong/PongGameOverScene.hpp"
#include "core/Game.hpp"

PongGameOverScene::PongGameOverScene(Game* game, int player1Score, int player2Score)
    : Scene(game), _Player1Score(player1Score), _Player2Score(player2Score), _DisplayTime(0.0f)
{
}

void PongGameOverScene::Init()
{
    _State = SceneState::ACTIVE;
}

SceneResult PongGameOverScene::Update(float deltaTime)
{
    _DisplayTime += deltaTime;
    // Check if time to return to menu
    if (_DisplayTime >= DISPLAY_DURATION) 
        return SceneResult::EXIT;
    return SceneResult::CONTINUE;
}

void PongGameOverScene::Draw()
{
    BeginDrawing();
    ClearBackground(_Game->GetClearColor());
    
    const char* gameOverText = "Game Over";

    const char* winnerText = _Player1Score >= _Player2Score ? "Player 1 Wins!" : "Player 2 Wins!";
    std::string scoreTextString = std::to_string(_Player1Score) + " - " + std::to_string(_Player2Score);
    const char* scoreText = scoreTextString.c_str();
    
    Font font = GetFontDefault();    
    FVector2 centerScreen = _Game->GetScreenSize() * 0.5f;
    
    Vector2 gameOverTextSize = MeasureTextEx(font, gameOverText, 20, 2);
    Vector2 winnerTextSize = MeasureTextEx(font, winnerText, 20, 2);
    Vector2 scoreTextSize = MeasureTextEx(font, scoreText, 20, 2);

    Vector2 gameOverTextOrigin = { gameOverTextSize.x * 0.5f, gameOverTextSize.y * 0.5f };
    Vector2 winnerTextOrigin = { winnerTextSize.x * 0.5f, winnerTextSize.y * 0.5f };
    Vector2 scoreTextOrigin = { scoreTextSize.x * 0.5f, scoreTextSize.y * 0.5f };

    DrawTextPro(font, gameOverText, centerScreen, gameOverTextOrigin, 0, 20, 2, RAYWHITE);
    DrawTextPro(font, winnerText, FVector2(centerScreen.x, centerScreen.y + 30), winnerTextOrigin, 0, 20, 2, RAYWHITE);
    DrawTextPro(font, scoreText, FVector2(centerScreen.x, centerScreen.y + 60), scoreTextOrigin, 0, 20, 2, RAYWHITE);

    EndDrawing();
}

void PongGameOverScene::Destroy()
{
    _State = SceneState::UNLOADING;
}
