#pragma once
#include "core/GameObject.hpp"
#include <functional>
#include <vector>
#include <memory>

class Player;

class ScoreUI : public GameObject{
private:
    int _MaxPlayers;
    std::vector<Player*> _Players; // Raw pointers to player objects (owned by game)
public:
    std::function<void(int)> ScoreEvent;
public:
    ScoreUI(Game* game, int maxPlayers = 4, FVector2 position = FVector2{0,0}, FVector2 size = FVector2{1,1}, Color color = RAYWHITE);
    ~ScoreUI();
    
    virtual void Update(float deltaTime) override;
    virtual void UpdateControlled(float deltaTime) override;
    virtual void Draw() override;
    virtual void Start() override;

    void UpdateScore(const int playerIndex);
    int GetPlayerScore(const int playerIndex) const;
    int GetNumberOfPlayers() const;
};