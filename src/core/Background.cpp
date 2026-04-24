#include "core/Background.hpp"
#include "core/Game.hpp"


Background::Background(Game* game, const FVector2 position, Color color) : 
    GameObject(game, position, color) {
    _DrawLayer = DrawLayer::BACKGROUND;
}

void Background::Update(float deltaTime){
    // PongGame* pongGame = dynamic_cast<PongGame*>(_Game);
    // _ScoreLeft = pongGame->GetPlayerPoints(0);
    // _ScoreRight = pongGame->GetPlayerPoints(1);
}
void Background::Draw(){
    GameObject::Draw();
    FVector2 screenSize = _Game->GetScreenSize();
    DrawRectangle(_Position.x, _Position.y, screenSize.x, screenSize.y, _Color);
    // DrawText(std::to_string(_ScoreLeft).c_str(), (int)_Position.x, (int)_Position.y, _Size.x, _Color);
    // DrawText(std::to_string(_ScoreRight).c_str(), _Game->GetScreenSize().x - (int)_Position.x, (int)_Position.y, _Size.x, _Color);
}
void Background::Start(){
    // _ScoreLeft = 0;
    // _ScoreRight = 0;
    // PongGame* pongGame = dynamic_cast<PongGame*>(_Game);
    // // Using std::bind
    // // pongGame->ScoreEvent = std::bind(&ScoreUI::UpdateScore, this, std::placeholders::_1);
    // pongGame->ScoreEvent = [this](int playerIndex){
    //     UpdateScore(playerIndex);
    // };
}