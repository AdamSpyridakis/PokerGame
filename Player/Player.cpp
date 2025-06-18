#include "Player.hpp"

Player::Player(int playerIndex, CommonVariables *variables) {
    m_playerIndex = playerIndex;
    std::cout << "Player " << m_playerIndex << " enter your name: ";
    std::cin >> m_playerName;

    m_variables = variables;
}

ActionType Player::takeBets(unsigned int amount, bool isForced) {
    // If it is not a forced bet (such as a blind), poll the player
    PlayerAction action = {ActionType::Blind, amount};
    if (!isForced) {
        action = pollPlayer(amount);
    }

    // go all in if player tries to bet more than they have
    if (action.betAmount >= m_stack) {
        m_contributionToCurrentHand += m_stack;
        m_stack = 0;
        return ActionType::AllIn;
    }

    m_contributionToCurrentHand += action.betAmount;
    m_stack -= action.betAmount;
    return action.action;
}


PlayerAction Player::pollPlayer(unsigned int amountToCall) {
    ValidActions valid = getValidActions(amountToCall);
    printValidActions(valid, amountToCall);
    PlayerAction returnVal = {};
    while (true) {
        std::string input;
        std::cin >> input;
        ActionType action = getActionType(input, valid);
        switch (action) {
            case Call:
                returnVal.action = ActionType::Call;
                returnVal.betAmount = amountToCall;
                return returnVal;
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
                    returnVal.action = ActionType::Raise;
                    returnVal.betAmount = raiseAmount + amountToCall;
                    return returnVal;
                }
            case AllIn:
                returnVal.action = ActionType::AllIn;
                returnVal.betAmount = m_stack;
                return returnVal;
            case Fold:
                returnVal.action = ActionType::Fold;
                returnVal.betAmount = 0;
                m_isFolded = true;
                return returnVal;
            case Check:
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

    // Make conditionals >=, if = the takeBets function will return the AllIn action
    if (amountToCall == 0) {
        returnVal.canCheck = true;
    } else {
        returnVal.canCall = (m_stack >= amountToCall);
    }
    returnVal.canRaise = (m_stack >= m_variables->minimumRaiseAmount);
    return returnVal;
}

void Player::printValidActions(ValidActions valid, unsigned int amountToCall) {
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
        return Call;
    } else if (input == "raise" && valid.canRaise) {
        return Raise;
    } else if (input == "allIn") {
        return AllIn;
    } else if (input == "fold") {
        return Fold;
    } else if (input == "check" && valid.canCheck) {
        return Check;
    } else {
        // don't poll player on blind, so treat as default action
        return Blind;
    }
}