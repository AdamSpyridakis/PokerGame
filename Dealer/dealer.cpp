#include "dealer.hpp"

Dealer::Dealer() {
    initDeck();
    deckIndex = 0;
}

Dealer::~Dealer() {
    delete[] m_deck;
}

Hand Dealer::dealPlayerHand() {
    Hand playerHand = {m_deck[deckIndex], m_deck[deckIndex + 1]};
    deckIndex+=2;
    return playerHand;
}

Card Dealer::dealCard() {
    Card card = m_deck[deckIndex];
    deckIndex++;
    return card;
}

void Dealer::initDeck() {
    m_deck = new Card[52];
    for (int i = Suit::Diamonds; i <= Suit::Spades; ++i) {
        for (int j = Value::Ace; j <= Value::King; ++j) {
            m_deck[i*13 + j] = {static_cast<Suit>(i), static_cast<Value>(j)};
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
        Card temp = m_deck[randomDeckSpot];
        m_deck[randomDeckSpot] = m_deck[i];
        m_deck[i] = temp;
    }
}
