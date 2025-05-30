#pragma once

#include "../Common/CommonTypes.hpp"
#include <iostream>

enum ActionType {
    Call,
    Raise,
    Fold,
    Check,
    Invalid
};

class Player {
public:
    Player(int playerIndex, CommonVariables *variables);

    bool takeBets(int amount, bool isForced);
    int pollPlayer(int amountToCall);
    ActionType getActionType(std::string input);

    Player *m_nextPlayer;
    int m_playerIndex;
    std::string m_playerName;
    int m_stack = startingStack;
    int m_contributionToCurrentHand = 0;
    Hand m_playerHand;
    bool m_isFolded = false;

private:
    CommonVariables *m_variables;
};