#pragma once
#include "core/UIObject.hpp"
#include <functional>
#include <vector>
#include <memory>

class Player;

// Event signature for score updates: (playerIndex, score)
using ScoreEventCallback = std::function<void(int, int)>;

class ScoreUI : public UIObject<ScoreEventCallback>
{
private:
    int _MaxPlayers;
    std::vector<Player*> _Players; // Raw pointers to player objects (owned by game)
public:
    ScoreUI(Game* game, int maxPlayers = 4, FVector2 position = FVector2{0,0}, FVector2 size = FVector2{1,1}, Color color = RAYWHITE);
    ~ScoreUI();
    
    virtual void Update(float deltaTime) override;
    virtual void UpdateControlled(float deltaTime) override;
    virtual void Draw() override;
    virtual void Start() override;

    void AddPlayer(Player* player);
    void UpdateScore(const int playerIndex, const int points);
    int GetPlayerScore(const int playerIndex) const;
    int GetNumberOfPlayers() const;
};