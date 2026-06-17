#pragma once
#include "core/UIObject.hpp"

class Timer : public UIObject<std::function<void()>>
{
private:
    float _ElapsedTime; // Timer in seconds
public:
    Timer(Game* game, FVector2 position = FVector2{0, 0}, FVector2 size = FVector2{20, 20}, Color color = RAYWHITE);
    ~Timer();
    
    virtual void Update(float deltaTime) override;
    virtual void Draw() override;
    virtual void Start() override;
    
    float GetElapsedTime() const { return _ElapsedTime; }
    void ResetTimer() { _ElapsedTime = 0.0f; }
};
