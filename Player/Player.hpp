#pragma once

#include "../commonTypes.hpp"
#include <iostream>

class Player {
public:
    Player();
    ~Player() = default;

    void setPlayerName();

    void setPlayerHand(Hand playerHand);
    Hand getPlayerHand();

    int getContribution();

    bool getFoldStatus();

    int takeBets(int amount);

private:
    std::string m_playerName;
    int m_stack;
    int m_contributionToCurrentHand;
    Hand m_playerHand;
    bool m_isFolded;
};