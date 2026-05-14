#pragma once
#include "core/UIObject.hpp"
#include <functional>

// Event signature for stat changes: (valueToSET, e.g. current life or score)
using StatBarEventCallback = std::function<void(int)>;

// A generic UI bar that visualizes a stat with a maximum (e.g. life points, mana points).
// The filled portion scales relative to the current/max ratio.
// Use AddPoints / RemovePoints / SetCurrent to mutate; UpdateEvent fires whenever the
// value changes, OnDepleted fires when current reaches 0, OnFilled when current reaches max.
class StatBar : public UIObject<StatBarEventCallback> {
protected:
    int _MaxPoints;
    int _CurrentPoints;
    
private:
    Color _BackgroundColor;
    Color _BorderColor;
    int _BorderThickness;
    bool _ShowText;

public:
    StatBar(Game* game,
            int maxPoints = 100,
            int currentPoints = 100,
            FVector2 position = FVector2{0, 0},
            FVector2 size = FVector2{200, 20},
            Color fillColor = RED,
            Color backgroundColor = DARKGRAY,
            Color borderColor = BLACK);
    ~StatBar();

    virtual void Update(float deltaTime) override;
    virtual void UpdateControlled(float deltaTime) override;
    virtual void Draw() override;
    virtual void Start() override;

    // Mutators (all clamp to [0, max] and fire events when value changes).
    void AddPoints(int amount);
    void RemovePoints(int amount);
    void SetCurrent(int value);
    void Refill();

    // Display options.
    void SetBorderThickness(int thickness);
    void SetShowText(bool show);
    void SetBackgroundColor(Color color);
    void SetBorderColor(Color color);

    // Accessors.
    int GetCurrent() const;
    int GetMax() const;
    float GetRatio() const; // current / max in [0,1]
    bool IsDepleted() const;
    bool IsFull() const;
};
