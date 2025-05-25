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
    void setupLinkedList();
    void dealPlayerHands();
    void takeInitialBets();
    void beginBetting();

    void updatePot(int betAmount, Player *player);
    void updateSidePots(Player *player);

    Dealer *m_dealer;
    std::vector<Player *> m_players;
    int m_numPlayers;
    Player *m_buttonPlayer;

    std::vector<Pot> m_pot;
    int maxPot;

    int smallBlind = 1;
    int bigBlind = 2;

    void printPlayers();
};
