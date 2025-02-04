#include "Player.hpp"

Player::Player(int playerIndex) {
    m_playerIndex = playerIndex;
#ifdef CONSOLE_PRINT
    std::cout << "Player " << m_playerIndex << ". Enter your name: ";
    std::cin >> m_playerName;
#endif
}

bool Player::takeBets(int amount) {
    // go all in if player tries to bet more than they have
    if (amount > m_stack) {
        m_contributionToCurrentHand += m_stack;
        m_stack = 0;
        return true;
    }

    m_contributionToCurrentHand += amount;
    m_stack -= amount;
    return false;
}