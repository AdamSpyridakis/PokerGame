#pragma once

#include "../commonTypes.hpp"
#include <iostream>

class Player {
private:
    Player(int playerIndex);

    bool takeBets(int amount);
    int pollPlayer(int amountToCall);

    Player *m_nextPlayer;
    int m_playerIndex;
    std::string m_playerName;
    int m_stack = startingStack;
    int m_contributionToCurrentHand;
    Hand m_playerHand;
    bool m_isFolded = false;

    friend class GameLogic;
};