#include "../commonTypes.hpp"
#include "../Player/Player.hpp"
#include <random>

#pragma once

class Dealer {
public:
    Dealer();
    ~Dealer();

    void dealPlayerHand(Player *player);

private:
    void initDeck();
    void shuffleDeck();

    Card *m_deck;
    int deckIndex;
};