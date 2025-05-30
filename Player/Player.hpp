#pragma once

#include "../Common/CommonTypes.hpp"
#include <iostream>

enum ActionType {
    Call,
    Raise,
    AllIn,
    Fold,
    Check,
    Invalid
};

class Player {
public:
    Player(int playerIndex, CommonVariables *variables);

    bool takeBets(unsigned int amount, bool isForced);
    unsigned int pollPlayer(unsigned int amountToCall);
    ActionType getActionType(std::string input);

    Player *m_nextPlayer;
    int m_playerIndex;
    std::string m_playerName;
    unsigned int m_stack = startingStack;
    unsigned int m_contributionToCurrentHand = 0;
    Hand m_playerHand;
    bool m_isFolded = false;

private:
    CommonVariables *m_variables;
};