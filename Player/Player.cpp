#include "Player.hpp"

static const std::string LOG_TAG = "GameLogic";

Player::Player(int playerIndex, CommonVariables *variables) {
    m_playerIndex = playerIndex;
    logEssential(std::format("Player {} enter your name: ", m_playerIndex));
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
                    logError(LOG_TAG, "Not enough chips!");
                    break;
                } else if (raiseAmount < m_variables->minimumRaiseAmount) {
                    logError(LOG_TAG, std::format("Minimum raise amount is {}!", m_variables->minimumRaiseAmount));
                    break;
                } else {
                    returnVal.action = ActionType::Raise;
                    returnVal.betAmount = raiseAmount + amountToCall;
                    m_variables->minimumRaiseAmount = raiseAmount;
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
                logError(LOG_TAG, "Invalid action type!");
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
    logEssential(std::format("Player {} it is your action!", m_playerIndex));
    logEssential("Type fold to fold.");
    logEssential("Type allIn to go all in.");
    if (valid.canCall) {
        logEssential(std::format("Type call to call {}.", amountToCall));
    }
    if (valid.canRaise) {
        logEssential(std::format("Type raise and a number to raise. Minimum raise: {}", m_variables->minimumRaiseAmount));
    }
    if (valid.canCheck) {
        logEssential("Type check to check.");
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