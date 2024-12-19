#pragma once

#include "../Player/Player.hpp"
#include "../Dealer/Dealer.hpp"

class GameLogic {
public:
    GameLogic(int numPlayers);
    ~GameLogic();

private:
    void startGame();

    Dealer *m_dealer;
    Player *m_players;
    int m_numPlayers;
};
