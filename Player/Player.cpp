#include "Player.hpp"

Player::Player() {
    m_stack = startingStack;
}

void Player::setPlayerName() {
    std::cin >> m_playerName;
}

void Player::setPlayerHand(Hand playerHand) {
    m_playerHand = playerHand;
}

Hand Player::getPlayerHand() {
    return m_playerHand;
}

int Player::getContribution() {
    return m_contributionToCurrentHand;
}

bool Player::getFoldStatus() {
    return m_isFolded;
}

int Player::takeBets(int amount) {
    // go all in if player tries to bet more than they have
    if (amount > m_stack) {
        m_contributionToCurrentHand += m_stack;
        m_stack = 0;
        return m_stack;
    }

    m_contributionToCurrentHand += amount;
    m_stack -= amount;
    return amount;
}