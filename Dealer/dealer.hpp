#include "../commonTypes.hpp"

#pragma once

class Dealer {
public:
    Dealer();
    ~Dealer() = default;

private:
    void initDeck();
    void shuffleDeck();

    const Card *deck;
};