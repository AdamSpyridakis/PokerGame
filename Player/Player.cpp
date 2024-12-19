#include "Player.hpp"

Player::Player() {
    m_bankroll = 1000;
}

void Player::setPlayerHand(Hand playerHand) {
    m_playerHand = playerHand;
}

Hand Player::getPlayerHand() {
    return m_playerHand;
}