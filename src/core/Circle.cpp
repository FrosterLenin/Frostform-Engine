#include "core/Circle.hpp"

Circle::Circle(Game* game, FVector2 position, float radius, Color color) : 
    GameObject(game, position, color), 
    _Radius(radius){
    _Size = {radius * 2, radius * 2};
}
Circle::Circle(Game* game, float accelerationIndex, FVector2 position, float radius, Color color) : 
    Circle(game, position, radius, color){
    _AccelerationIndex = accelerationIndex;
}

Circle::~Circle(){

}

void Circle::Update(float deltaTime){
    if(!_Game) return;
}
void Circle::Draw(){
    GameObject::Draw();
    DrawCircle(GetCenter().x, GetCenter().y, _Radius, _Color);
}


void Circle::SetRadius(const float other){
    _Radius = other;
    _Size.x = other;
    _Size.y = other;
}
float Circle::GetRadius() const{
    return _Radius;
}
// NOT USED, MANAGED BY COLLISION MANAGER AND OnCollisionEnter/Exit/Stay METHODS
bool Circle::CheckCollision(const GameObject& other) const{

    const Circle* otherCircle = dynamic_cast<const Circle*>(&other);
    if(otherCircle != nullptr){
        // Distance between the two circles is less than the sum of the two rays
        return FVector2::Distance(this->_Position, other.GetPosition()) < this->_Radius + otherCircle->GetRadius();
    }
    return false;
}