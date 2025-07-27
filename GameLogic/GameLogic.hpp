#pragma once

#include "../Player/Player.hpp"
#include "../Dealer/Dealer.hpp"
#include "../Common/Logging.hpp"

#include <vector>
#include <format>
#include <memory>

class GameLogic {
public:
    GameLogic(int numPlayers);
    ~GameLogic();

private:
    void startGame();
    void setupLinkedList();
    void dealPlayerHands();
    void dealFlop();
    void dealTurn();
    void dealRiver();
    void startPreFlopBetting();
    void startPostFlopBetting();
    void startBettingRound(Player *currentBettingPlayer, int maxContribution);
    void cleanUpBettingRound();

    void recalculatePot();
    void takeBlind(unsigned int blindAmount, Player *player);
    unsigned int takeBet(unsigned int maxContibution, Player *player);
    void updateSidePots(Player *player);

    std::unique_ptr<Dealer> m_dealer;
    std::vector<Player *> m_players;
    int m_numPlayers;
    Player *m_buttonPlayer;

    Card *m_board;
    int m_cardsDealt;

    struct Pot {
        unsigned int amount;
        unsigned int maxBet;
        std::vector<Player *> players;
    };
    std::vector<Pot> m_pot;
    unsigned int m_maxPot;

    unsigned int m_bigBlind = startingBigBlind;
    unsigned int m_smallBlind = startingSmallBlind;

    // Round specific variables
    std::shared_ptr<CommonVariables> m_variables;
    unsigned int m_minimumRaise = m_bigBlind;
    int m_playersInHand;
    int m_playersActionComplete;

    void printPlayers();
    void printPot();
    void printBoard();
};
