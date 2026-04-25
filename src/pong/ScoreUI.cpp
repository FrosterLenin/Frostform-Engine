#include "pong/ScoreUI.hpp"
#include "pong/PongGame.hpp"
#include <string>

ScoreUI::ScoreUI(Game* game, FVector2 position, FVector2 size, Color color) : 
    GameObject(game, position, size, color)
    , _ScoreLeft(0)
    , _ScoreRight(0){
    // Initialize the event in constructor so it's available before Start() is called
    ScoreEvent = [this](int playerIndex){
        UpdateScore(playerIndex);
    };
}

ScoreUI::~ScoreUI() {}

void ScoreUI::Update(float deltaTime){
    GameObject::Update(deltaTime);
}
void ScoreUI::Draw(){
    GameObject::Draw();
    DrawText(std::to_string(_ScoreLeft).c_str(), (int)_Position.x, (int)_Position.y, _Size.x, _Color);
    DrawText(std::to_string(_ScoreRight).c_str(), _Game->GetScreenSize().x - (int)_Position.x, (int)_Position.y, _Size.x, _Color);
}
void ScoreUI::Start(){
    _ScoreLeft = 0;
    _ScoreRight = 0;
}

bool ScoreUI::CheckCollision(const GameObject& other) const {
    return false; // UI doesn't collide
}
void ScoreUI::UpdateScore(const int playerIndex){
    if(playerIndex == 0)
        _ScoreLeft += 1;
    else
        _ScoreRight += 1;
}
void ScoreUI::UpdateControlled(float deltaTime){
    GameObject::UpdateControlled(deltaTime);
    PongGame* pongGame = dynamic_cast<PongGame*>(_Game);
    _ScoreLeft = pongGame->GetPlayerPoints(0);
    _ScoreRight = pongGame->GetPlayerPoints(1);
}