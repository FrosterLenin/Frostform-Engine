#pragma once
#include "core/Game.hpp"

class PongGameScene;
class PongGameOverScene;

class PongGame : public Game{
public:
    PongGame(FVector2 screenSize);

    virtual void InitGame(const Color clearColor = BLACK) override;
    
    // Access to game scene for passing data between scenes
    PongGameScene* GetGameScene() const;
    
    // Set game scene (called by scene during Init)
    void SetGameScene(PongGameScene* scene);

private:
    PongGameScene* _GameScene;
    PongGameOverScene* _GameOverScene;
};