#include "Player.hpp"

Player::Player(int playerIndex) {
    m_playerIndex = playerIndex;
    std::cout << "Player " << m_playerIndex << ". Enter your name: ";
    std::cin >> m_playerName;
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

int Player::pollPlayer(int amount) {
    std::cout << "Press C to call " << amount << ".\n";
    std::cout << "Press R and an amount to raise." << "\n";
    std::cout << "Press F to fold.\n";
    while(true){
        std::string input;
        std::cin >> input;
        if (input == "C") {
            return amount;
        } else if (input == "R"){
            int raiseAmount;
            std::cin >> raiseAmount;
            /* Throw an error if the amount to be called + the raise amount is bigger
               than the player's stack. */
            if (raiseAmount + amount > m_stack) {
                std::cout << "error message";
                continue;
            } else {
                return raiseAmount + amount;
            }
        }
    }
        
        
}