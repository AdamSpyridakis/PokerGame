#include "Player.hpp"

Player::Player(int playerIndex, CommonVariables *variables) {
    m_playerIndex = playerIndex;
    std::cout << "Player " << m_playerIndex << " enter your name: ";
    std::cin >> m_playerName;

    m_variables = variables;
}

ActionType Player::takeBlind(unsigned int amount) {
    if (amount >= m_stack) {
        m_contributionToCurrentHand += m_stack;
        m_contributionToBettingRound += m_stack;
        m_stack = 0;
        return ActionType::AllIn;
    }

    m_contributionToCurrentHand += amount;
    m_contributionToBettingRound += amount;
    m_stack -= amount;
    return ActionType::Blind;
}

ActionType Player::takeBet(unsigned int amountToCall) {
    PlayerAction action = {ActionType::Blind, amountToCall};
    action = pollPlayer(amountToCall);

    m_contributionToCurrentHand += action.betAmount;
    m_contributionToBettingRound += action.betAmount;
    m_stack -= action.betAmount;
    return action.action;
}

PlayerAction Player::pollPlayer(unsigned int amountToCall) {
    ValidActions valid = getValidActions(amountToCall);
    PlayerAction returnVal = {};
    while (true) {
        printValidActions(valid, amountToCall);
        std::string input;
        std::cin >> input;
        ActionType action = getActionType(input, valid);
        switch (action) {
            case ActionType::Call:
                returnVal.action = ActionType::Call;
                // If the user calls, ensure they at most go all in.
                if (amountToCall > m_stack) {
                    returnVal.betAmount = m_stack;
                } else {
                    returnVal.betAmount = amountToCall;
                }
                return returnVal;
            case ActionType::Raise:
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
                    returnVal.action = ActionType::Raise;
                    returnVal.betAmount = raiseAmount + amountToCall;
                    m_variables->minimumRaiseAmount = returnVal.betAmount;
                    return returnVal;
                }
            // AllIn is not a handled action by the game logic, only an option for players.
            case ActionType::AllIn:
                /* Don't allow further action in the turn if the user goes all in for
                   less than minimum raise amount. */
                if (m_stack < amountToCall + m_variables->minimumRaiseAmount) {
                    returnVal.action = ActionType::Call;
                } else {
                    m_variables->minimumRaiseAmount = returnVal.betAmount;
                    returnVal.action = ActionType::Raise;
                }
                returnVal.betAmount = m_stack;
                return returnVal;
            case ActionType::Fold:
                returnVal.action = ActionType::Fold;
                returnVal.betAmount = 0;
                m_isFolded = true;
                return returnVal;
            case ActionType::Check:
                returnVal.action = ActionType::Check;
                returnVal.betAmount = 0;
                return returnVal;
            default:
                std::cout << "????\n";
                break;
        }
    }
}

ValidActions Player::getValidActions(unsigned int amountToCall) {
    ValidActions returnVal = {false, false, false};

    if (amountToCall == 0) {
        returnVal.canCheck = true;
    } else {
        returnVal.canCall = true;
    }
    returnVal.canRaise = (m_stack >= m_variables->minimumRaiseAmount);
    return returnVal;
}

void Player::printValidActions(ValidActions valid, unsigned int amountToCall) {
    std::cout << "Player " << m_playerIndex << " your action!\n";
    std::cout << "Type fold to fold.\n";
    std::cout << "Type allIn to go all in.\n";
    if (valid.canCall) {
        std::cout << "Type call to call " << amountToCall << ".\n";
    }
    if (valid.canRaise) {
        std::cout << "Type raise and a number to raise. Minimum raise: " << m_variables->minimumRaiseAmount << ".\n";
    }
    if (valid.canCheck) {
        std::cout << "Type check to check.\n";
    }
}

ActionType Player::getActionType(std::string input, ValidActions valid) {
    if (input == "call" && valid.canCall) {
        return ActionType::Call;
    } else if (input == "raise" && valid.canRaise) {
        return ActionType::Raise;
    } else if (input == "allIn") {
        return ActionType::AllIn;
    } else if (input == "fold") {
        return ActionType::Fold;
    } else if (input == "check" && valid.canCheck) {
        return ActionType::Check;
    } else {
        // don't poll player on blind, so treat as default action
        return ActionType::Blind;
    }
}