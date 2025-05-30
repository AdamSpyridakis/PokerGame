#include "Player.hpp"

Player::Player(int playerIndex, CommonVariables *variables) {
    m_playerIndex = playerIndex;
    std::cout << "Player " << m_playerIndex << " enter your name: ";
    std::cin >> m_playerName;

    m_variables = variables;
}

bool Player::takeBets(unsigned int amount, bool isForced) {
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

unsigned int Player::pollPlayer(unsigned int amountToCall) {
    if (amountToCall == 0) {
        std::cout << "Type raise and a number to raise. Minimum raise: " << m_variables->minimumRaiseAmount << ".\n";
        std::cout << "Type fold to fold.\n";
        std::cout << "Type allIn to go all in.\n";
        std::cout << "Type check to check.\n";
        while (true) {
            std::string input;
            std::cin >> input;
            ActionType action = getActionType(input);
            switch (action) {
                case Raise:
                    unsigned int raiseAmount;
                    std::cin >> raiseAmount;
                    /* Throw an error if the amount to be called + the raise amount is bigger
                    than the player's stack. Or if the player is trying to raise less than */
                    if (raiseAmount + amountToCall > m_stack) {
                        std::cout << "Not enough chips!";
                        break;
                    } else if (raiseAmount < m_variables->minimumRaiseAmount) {
                        std::cout << "Minimum raise amount is " << m_variables->minimumRaiseAmount << "!";
                        break;
                    } else {
                        return raiseAmount + amountToCall;
                    }
                case AllIn:
                    return m_stack;
                case Fold:
                    m_isFolded = true;
                    return 0;
                case Check:
                    return 0;
                default:
                    std::cout << "????\n";
                    break;
            }
        }
    } else if (amountToCall >= m_stack) {
        std::cout << "Type fold to fold.\n";
        std::cout << "Type allIn to go all in.\n";
        while (true) {
            std::string input;
            std::cin >> input;
            ActionType action = getActionType(input);
            switch (action) {
                case AllIn:
                    return m_stack;
                case Fold:
                    m_isFolded = true;
                    return 0;
                default:
                    std::cout << "????\n";
                    break;
            }
        }
    } else if (amountToCall + m_variables->minimumRaiseAmount >= m_stack) {
        std::cout << "Type call to call " << amountToCall << ".\n";
        std::cout << "Type fold to fold.\n";
        std::cout << "Type allIn to go all in.\n";
        while (true) {
            std::string input;
            std::cin >> input;
            ActionType action = getActionType(input);
            switch (action) {
                case Call:
                    return amountToCall;
                case AllIn:
                    return m_stack;
                case Fold:
                    m_isFolded = true;
                    return 0;
                default:
                    std::cout << "????\n";
                    break;
            }
        }
    } else {
        std::cout << "Type call to call " << amountToCall << ".\n";
        std::cout << "Type raise and a number to raise. Minimum raise: " << m_variables->minimumRaiseAmount << ".\n";
        std::cout << "Type fold to fold.\n";
        std::cout << "Type allIn to go all in.\n";
        std::cout << "Type check to check.\n";
        while (true) {
            std::string input;
            std::cin >> input;
            ActionType action = getActionType(input);
            switch (action) {
                case Call:
                    return amountToCall;
                case Raise:
                    unsigned int raiseAmount;
                    std::cin >> raiseAmount;
                    /* Throw an error if the amount to be called + the raise amount is bigger
                    than the player's stack. Or if the player is trying to raise less than */
                    if (raiseAmount + amountToCall > m_stack) {
                        std::cout << "Not enough chips!";
                        break;
                    } else if (raiseAmount < m_variables->minimumRaiseAmount) {
                        std::cout << "Minimum raise amount is " << m_variables->minimumRaiseAmount << "!";
                        break;
                    } else {
                        return raiseAmount + amountToCall;
                    }
                case AllIn:
                    return m_stack;
                case Fold:
                    m_isFolded = true;
                    return 0;
                case Check:
                    return 0;
                default:
                    std::cout << "????\n";
                    break;
            }
        }
    }
}

ActionType Player::getActionType(std::string input) {
    if (input == "call") {
        return Call;
    } else if (input == "raise") {
        return Raise;
    } else if (input == "allIn") {
        return AllIn;
    } else if (input == "fold") {
        return Fold;
    } else if (input == "check") {
        return Check;
    } else {
        return Invalid;
    }
}