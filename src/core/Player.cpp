#include "core/Player.hpp"

Player::Player(Game* game, FVector2 position, FVector2 size, Color color)
    : GameObject(game, position, size, color), _Score(0) {
}

Player::Player(Game* game, FVector2 position, FVector2 size, Color color, float accelerationIndex)
    : GameObject(game, position, size, color, accelerationIndex), _Score(0) {
}

int Player::GetScore() const {
    return _Score;
}

void Player::AddScore(int points) {
    if (points > 0)
        _Score += points;
}

void Player::SetScore(int score) {
    if (score >= 0)
        _Score = score;
}

void Player::ResetScore() {
    _Score = 0;
}
