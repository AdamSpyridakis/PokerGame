#pragma once

#include "../Player/Player.hpp"
#include "../Dealer/Dealer.hpp"

#include <vector>

struct Pot {
    unsigned int amount;
    unsigned int maxBet;
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

    void updatePot(unsigned int betAmount, Player *player, bool isForced);
    void updateSidePots(Player *player);

    Dealer *m_dealer;
    std::vector<Player *> m_players;
    int m_numPlayers;
    Player *m_buttonPlayer;

    std::vector<Pot> m_pot;
    unsigned int maxPot;

    CommonVariables *m_variables;
    unsigned int m_bigBlind = startingBigBlind;
    unsigned int m_smallBlind = startingSmallBlind;
    unsigned int m_minimumRaise = m_bigBlind;
    int m_playersInHand;

    void printPlayers();
    void printPot();
};
