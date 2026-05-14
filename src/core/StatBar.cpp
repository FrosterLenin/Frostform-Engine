#include "core/StatBar.hpp"
#include "core/Game.hpp"
#include <algorithm>
#include <string>

StatBar::StatBar(Game* game,
                 int maxPoints,
                 int currentPoints,
                 FVector2 position,
                 FVector2 size,
                 Color fillColor,
                 Color backgroundColor,
                 Color borderColor)
    : UIObject(game, position, size, fillColor)
    , _MaxPoints(std::max(1, maxPoints))
    , _CurrentPoints(std::clamp(currentPoints, 0, std::max(1, maxPoints)))
    , _BackgroundColor(backgroundColor)
    , _BorderColor(borderColor)
    , _BorderThickness(1)
    , _ShowText(true) {
}

StatBar::~StatBar() {}

void StatBar::Start() {}

void StatBar::Update(float deltaTime) {
}

void StatBar::UpdateControlled(float deltaTime) {
    GameObject::UpdateControlled(deltaTime);
}

void StatBar::Draw() {
    const int x = static_cast<int>(_Position.x);
    const int y = static_cast<int>(_Position.y);
    const int w = static_cast<int>(_Size.x);
    const int h = static_cast<int>(_Size.y);

    // Background
    DrawRectangle(x, y, w, h, _BackgroundColor);

    // Filled portion scaled by ratio
    const float ratio = GetRatio();
    const int filledWidth = static_cast<int>(w * ratio);
    if (filledWidth > 0)
        DrawRectangle(x, y, filledWidth, h, _Color);

    // Border
    if (_BorderThickness > 0)
        DrawRectangleLinesEx(Rectangle{(float)x, (float)y, (float)w, (float)h},
                             (float)_BorderThickness, _BorderColor);

    // Text overlay (e.g. "75 / 100")
    if (_ShowText) {
        std::string text = std::to_string(_CurrentPoints) + " / " + std::to_string(_MaxPoints);
        int fontSize = std::max(10, h - 4);
        int textWidth = MeasureText(text.c_str(), fontSize);
        int textX = x + (w - textWidth) / 2;
        int textY = y + (h - fontSize) / 2;
        DrawText(text.c_str(), textX, textY, fontSize, RAYWHITE);
    }
}

void StatBar::AddPoints(int amount) {
    if (amount == 0) return;
    SetCurrent(_CurrentPoints + amount);
}

void StatBar::RemovePoints(int amount) {
    if (amount == 0) return;
    SetCurrent(_CurrentPoints - amount);
}

void StatBar::SetCurrent(int value) {
    const int previous = _CurrentPoints;
    _CurrentPoints = std::clamp(value, 0, _MaxPoints);
    if (_CurrentPoints == previous) return;
}

void StatBar::Refill() {
    SetCurrent(_MaxPoints);
}

void StatBar::SetBorderThickness(int thickness) { _BorderThickness = std::max(0, thickness); }
void StatBar::SetShowText(bool show) { _ShowText = show; }
void StatBar::SetBackgroundColor(Color color) { _BackgroundColor = color; }
void StatBar::SetBorderColor(Color color) { _BorderColor = color; }

int   StatBar::GetCurrent() const { return _CurrentPoints; }
int   StatBar::GetMax() const { return _MaxPoints; }
float StatBar::GetRatio() const { return _MaxPoints > 0 ? (float)_CurrentPoints / (float)_MaxPoints : .0f; }
bool  StatBar::IsDepleted() const { return _CurrentPoints <= 0; }
bool  StatBar::IsFull() const { return _CurrentPoints >= _MaxPoints; }
