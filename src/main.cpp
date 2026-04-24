#include "raylib.h"
#include "core/Game.hpp"
#include "pong/PongGame.hpp"
#include "SpaceInvaders/SpaceInvaders.hpp"

void PlayPong(){
    PongGame pongGame(FVector2{800,450});
    pongGame.InitGame(BLACK);
    pongGame.Run();
}
void PlaySpaceInvaders(){
    SpaceInvaders spaceInvaders;
    spaceInvaders.InitGame();
    spaceInvaders.Run();
}

int main(){

    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Demo Launcher");
    SetTargetFPS(60);

    const char* title = "Welcome to the Game Engine Demo!";
    int titleFontSize = 20;
    int titleWidth = MeasureText(title, titleFontSize);

    int paddingLeft = 20;

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        // Centered title
        DrawText(
            title,
            (screenWidth - titleWidth) / 2,
            40,
            titleFontSize,
            WHITE
        );

        // Left-side menu
        DrawText("Try demo games [PRESS KEY]:", paddingLeft, 150, 20, WHITE);
        DrawText("[P] Pong", paddingLeft, 190, 20, WHITE);
        DrawText("[S] Space Invaders", paddingLeft, 230, 20, WHITE);

        EndDrawing();

        if (IsKeyPressed(KEY_P)) {
            CloseWindow();
            PlayPong();
            break;
        }

        if (IsKeyPressed(KEY_S)) {
            CloseWindow();
            PlaySpaceInvaders();
            break;
        }
    }

    return 0;
}   