#include "dealer.hpp"
#include <random>

Dealer::Dealer() {
    initDeck();
}

Dealer::~Dealer() {
    delete[] deck;
}

void Dealer::initDeck() {
    deck = new Card[52];
    for (int i = Suit::Diamonds; i <= Suit::Spades; ++i) {
        for (int j = Value::Ace; j <= Value::King; ++j) {
            deck[i*13 + j] = {static_cast<Suit>(i), static_cast<Value>(j)};
        }
    }

    shuffleDeck();
}

void Dealer::shuffleDeck() {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(0, 51);

    for (int i = 0; i <= 51; ++i) {
        int randomDeckSpot = dist6(rng);
        Card temp = deck[randomDeckSpot];
        deck[randomDeckSpot] = deck[i];
        deck[i] = temp;
    }
}