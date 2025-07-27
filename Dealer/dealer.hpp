#pragma once

#include "../Common/commonTypes.hpp"
#include "../Player/Player.hpp"

#include <random>

class Dealer {
public:
    Dealer();
    ~Dealer();

    Hand dealPlayerHand();
    Card dealCard();

private:
    void initDeck();
    void shuffleDeck();

    Card *m_deck;
    int deckIndex;
};