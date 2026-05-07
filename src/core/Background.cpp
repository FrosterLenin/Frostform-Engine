#include "core/Background.hpp"
#include "core/Game.hpp"


Background::Background(Game* game, const FVector2 position, Color color) : 
    GameObject(game, position, color) {
    _DrawLayer = DrawLayer::BACKGROUND;
}

void Background::Update(float deltaTime){
}
void Background::Draw(){
    FVector2 screenSize = _Game->GetScreenSize();
    DrawRectangle(_Position.x, _Position.y, screenSize.x, screenSize.y, _Color);
}
void Background::Start(){

}