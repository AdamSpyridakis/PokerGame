#pragma once

#include "../Common/CommonTypes.hpp"
#include "../Common/Logging.hpp"

#include <format>
#include <memory>

class Player {
public:
    Player(int playerIndex, std::shared_ptr<CommonVariables> variables);

    ActionType takeBlind(unsigned int amount);
    ActionType takeBet(unsigned int amountToCall);

    Player *m_nextPlayer;
    int m_playerIndex;
    std::string m_playerName;
    unsigned int m_stack = startingStack;
    unsigned int m_contributionToCurrentHand = 0;
    Hand m_playerHand;
    bool m_isFolded = false;

    unsigned int m_contributionToBettingRound = 0;

private:
    struct PlayerAction {
        ActionType action;
        unsigned int betAmount;
    };

    struct ValidActions {
        bool canCall;
        bool canRaise;
        bool canCheck;
    };

    ValidActions getValidActions(unsigned int amountToCall);
    void printValidActions(ValidActions valid, unsigned int amountToCall);
    PlayerAction pollPlayer(unsigned int amountToCall);
    ActionType getActionType(std::string input, ValidActions valid);
    std::shared_ptr<CommonVariables> m_variables;
};