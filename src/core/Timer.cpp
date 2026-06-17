#include "core/Timer.hpp"
#include "core/Game.hpp"
#include <string>

Timer::Timer(Game* game, FVector2 position, FVector2 size, Color color) :
    UIObject(game, position, size, color),
    _ElapsedTime(0.0f) {
}

Timer::~Timer() {}

void Timer::Update(float deltaTime) {
    _ElapsedTime += deltaTime;
}

void Timer::Draw() {
    float screenWidth = _Game->GetScreenSize().x;
    
    // Draw timer at top center
    int minutes = static_cast<int>(_ElapsedTime) / 60;
    int seconds = static_cast<int>(_ElapsedTime) % 60;
    std::string timerText = TextFormat("%02d:%02d", minutes, seconds);
    int timerWidth = MeasureText(timerText.c_str(), static_cast<int>(_Size.x));
    DrawText(timerText.c_str(), static_cast<int>(screenWidth / 2.0f - timerWidth / 2.0f), static_cast<int>(_Position.y), static_cast<int>(_Size.x), _Color);
}

void Timer::Start() {
    SetActive(true);
    _ElapsedTime = 0.0f;
}
