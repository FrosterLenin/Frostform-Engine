#pragma once

#include "core/Scene.hpp"

class Background;
class RasterizerTriangleObject;
class RasterizerModeUI;

class RasterizerDemoScene : public Scene
{
public:
    // explicit constructor to avoid accidental implicit conversions
    // since Scene constructors can take multiple parameters
    // so current class can only be constructed with a Game* argument, and not accidentally with other types
    explicit RasterizerDemoScene(Game* game);
    virtual ~RasterizerDemoScene() = default;

    virtual void Init() override;
    virtual SceneResult Update(float deltaTime) override;
    virtual void Draw() override;
    virtual void Destroy() override;

private:
    void DrawCameraInfo();

    Background* _Background;
    RasterizerTriangleObject* _Triangle;
    RasterizerModeUI* _ModeUI;
    int _LastMode;
};
