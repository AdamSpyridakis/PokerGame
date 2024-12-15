#pragma once

#include <string>

enum Suit {
    Diamonds,
    Hearts,
    Clubs,
    Spades
};
std::string toStrSuit(Suit suit);

enum Value {
    Ace,
    Two,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Nine,
    Ten,
    Jack,
    Queen,
    King
};
std::string toStrValue(Value value);

struct Card {
    Suit suit;
    Value value;
};
std::string toStrCard(Card card);