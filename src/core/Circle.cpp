#include "core/Circle.hpp"
#include "core/Game.hpp"
#include "core/enums/RasterMode.hpp"
#include "core/rasterizer/Rasterizer.hpp"

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
    if (_Game == nullptr)
        return;

    const RasterMode rasterMode = _Game->GetRasterMode();
    if (rasterMode == RasterMode::BBOX_TRIANGLE && _Game->GetScreen() != nullptr) {
        const FVector2 center = GetCenter();
        Rasterizer::DrawCircle(
            IVector2{static_cast<int>(center.x), static_cast<int>(center.y)}
            , static_cast<int>(_Radius)
            , _Color
            , _Game->GetScreen()
        );
        return;
    }

    DrawCircle(static_cast<int>(GetCenter().x), static_cast<int>(GetCenter().y), _Radius, _Color);
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