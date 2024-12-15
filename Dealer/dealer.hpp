#include "../commonTypes.hpp"

#pragma once

class Dealer {
public:
    Dealer();
    ~Dealer();

private:
    void initDeck();
    void shuffleDeck();

    Card *deck;
};