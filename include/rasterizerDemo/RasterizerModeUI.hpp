#pragma once

#include "core/UIObject.hpp"

#include <functional>

using RasterizerModeEvent = std::function<void(int)>;

// Simple UI object to display the current rasterizer mode and instructions to change it
class RasterizerModeUI : public UIObject<RasterizerModeEvent>
{
public:
    RasterizerModeUI(Game* game, FVector2 position = FVector2{10.0f, 10.0f},
                     FVector2 size = FVector2{20.0f, 20.0f}, Color color = WHITE);
    virtual ~RasterizerModeUI() = default;

    virtual void Start() override;
    virtual void Update(float deltaTime) override;
    virtual void Draw() override;

    void SetMode(int mode);

private:
    int _Mode; // 1 = wireframe, 2 = filled, 3 = filled with vertex colors
};
