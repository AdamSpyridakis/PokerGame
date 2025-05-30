#include "Player.hpp"

Player::Player(int playerIndex) {
    m_playerIndex = playerIndex;
    std::cout << "Player " << m_playerIndex << ". Enter your name: ";
    std::cin >> m_playerName;
}

bool Player::takeBets(int amount, bool isForced) {
    // If it is not a forced bet (such as a blind), poll the player
    if (!isForced) {
        amount = pollPlayer(amount);
    }

    // go all in if player tries to bet more than they have
    if (amount >= m_stack) {
        m_contributionToCurrentHand += m_stack;
        m_stack = 0;
        return true;
    }

    m_contributionToCurrentHand += amount;
    m_stack -= amount;
    return false;
}

int Player::pollPlayer(int amountToCall) {
    std::cout << "Press C to call " << amountToCall << ".\n";
    std::cout << "Press R and an amount to raise." << "\n";
    std::cout << "Press F to fold.\n";
    while (true) {
        std::string input;
        std::cin >> input;
        ActionType action = getActionType(input);
        switch (action) {
            case Call:
                return amountToCall;
            case Raise:
                int raiseAmount;
                std::cin >> raiseAmount;
                /* Throw an error if the amount to be called + the raise amount is bigger
                than the player's stack. */
                if (raiseAmount + amountToCall > m_stack) {
                    std::cout << "error message";
                    break;
                } else {
                    return raiseAmount + amountToCall;
                }
            case Fold:
                m_isFolded = true;
                return 0;
            case Check:
                if (amountToCall != 0) {
                    std::cout << "error message";
                }
            case Invalid:
                break;
        }
    }
}

ActionType Player::getActionType(std::string input) {
    if (input == "call") {
        return Call;
    } else if (input == "raise") {
        return Raise;
    } else if (input == "fold") {
        return Fold;
    } else if (input == "check") {
        return Check;
    } else {
        return Invalid;
    }
}