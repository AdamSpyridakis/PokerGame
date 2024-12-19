#pragma once

#include "../commonTypes.hpp"

class Player {
public:
    Player();
    ~Player() = default;

    void setPlayerHand(Hand playerHand);
    Hand getPlayerHand();

private:
    int m_bankroll;
    Hand m_playerHand;
};