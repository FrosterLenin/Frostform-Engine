#pragma once

#include "core/Game.hpp"

class RasterizerDemoScene;

// Rasterizer demo game built on top of the shared Game/Scene architecture.
class RasterizerDemo : public Game {
public:
    RasterizerDemo(FVector2 screenSize = FVector2{800, 450});

    virtual void InitGame(const Color clearColor = BLACK) override;

    RasterizerDemoScene* GetGameScene() const;
    void SetGameScene(RasterizerDemoScene* scene);

private:
    RasterizerDemoScene* _GameScene;
};
