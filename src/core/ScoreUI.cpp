#include "core/ScoreUI.hpp"
#include "core/Player.hpp"
#include "core/Game.hpp"
#include <string>
#include <algorithm>

ScoreUI::ScoreUI(Game* game, int maxPlayers, FVector2 position, FVector2 size, Color color) : 
    GameObject(game, position, size, color)
    , _MaxPlayers(std::min(maxPlayers, 4)) { // Cap at 4 players max
}

ScoreUI::~ScoreUI() {}

void ScoreUI::Update(float deltaTime){
    GameObject::Update(deltaTime);
}

void ScoreUI::Draw(){
    GameObject::Draw();
    
    float screenWidth = _Game->GetScreenSize().x;
    float screenHeight = _Game->GetScreenSize().y;
    
    int numberOfPlayers = static_cast<int>(_Players.size());
    
    // Calculate x positions based on number of players
    std::vector<float> xPositions(numberOfPlayers);
    
    switch(numberOfPlayers) {
        case 1:
            // Single player: top left
            xPositions[0] = _Position.x;
            break;
        case 2:
            // Two players: top left and top right
            xPositions[0] = _Position.x;
            xPositions[1] = screenWidth - _Position.x - 50; // Offset for text width
            break;
        case 3:
            // Three players: top left, top middle, top right
            xPositions[0] = _Position.x;
            xPositions[1] = screenWidth / 2 - 25; // Center
            xPositions[2] = screenWidth - _Position.x - 50;
            break;
        case 4:
            // Four players: top corners and two in top middle (justified)
            xPositions[0] = _Position.x;
            xPositions[1] = screenWidth / 3 - 25;
            xPositions[2] = 2 * screenWidth / 3 - 25;
            xPositions[3] = screenWidth - _Position.x - 50;
            break;
    }
    
    int fontSize = (int)_Size.x;
    // Draw each player's score
    for(int i = 0; i < numberOfPlayers; ++i) {
        if(_Players[i]) {
            std::string scoreText = std::to_string(_Players[i]->GetScore());
            DrawText(scoreText.c_str(), (int)xPositions[i], (int)_Position.y, fontSize, _Color);
        }
    }
}

void ScoreUI::Start(){
    // Get all players from the game (up to _MaxPlayers)
    std::vector<Player*> allPlayers = _Game->GetPlayers();
    _Players.clear();
    
    // Store pointers to players up to the max limit
    for(int i = 0; i < std::min(static_cast<int>(allPlayers.size()), _MaxPlayers); ++i)
        _Players.push_back(allPlayers[i]);
}


void ScoreUI::UpdateScore(const int playerIndex, const int points){
    if(playerIndex >= 0 && playerIndex <static_cast<int>(_Players.size())){
        if(_Players[playerIndex])
            _Players[playerIndex]->AddScore(points);
    }
}

int ScoreUI::GetPlayerScore(const int playerIndex) const {
    if(playerIndex >= 0 && playerIndex < static_cast<int>(_Players.size())){
        if(_Players[playerIndex])
            return _Players[playerIndex]->GetScore();
    }
    return 0;
}

void ScoreUI::UpdateControlled(float deltaTime){
    GameObject::UpdateControlled(deltaTime);
    // Update player references in case they changed (though they shouldn't)
    if(_Game) {
        std::vector<Player*> allPlayers = _Game->GetPlayers();
        if(allPlayers.size() != _Players.size()) {
            _Players.clear();
            for(int i = 0; i < std::min(static_cast<int>(allPlayers.size()), _MaxPlayers); ++i)
                _Players.push_back(allPlayers[i]);
        }
    }
}
int ScoreUI::GetNumberOfPlayers() const {
    return static_cast<int>(_Players.size());
}