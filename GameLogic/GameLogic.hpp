#pragma once

#include "../Player/Player.hpp"
#include "../Dealer/Dealer.hpp"
#include <vector>

struct Pot {
    int amount;
    int maxBet;
    std::vector<Player *> players;
};

class GameLogic {
public:
    GameLogic(int numPlayers);
    ~GameLogic();

private:
    void startGame();
    void dealPlayerHands();
    void takeInitialBets();
    void beginBetting();

    void updatePot(int betAmount, Player *player);
    void updateSidePots(int betAmount, Player *player);

    Dealer *m_dealer;
    std::vector<Player *> m_players;
    int m_numPlayers;
    int m_buttonPlayer;

    std::vector<Pot> m_pot;
    int maxPot;
};
