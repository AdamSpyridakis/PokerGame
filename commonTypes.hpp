#pragma once

#include <string>
#include <vector>

const int startingStack = 1000;
const int smallBlind = 1;
const int bigBlind = 2;

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

struct Hand {
    Card firstCard;
    Card secondCard;
};
std::string toStrHand(Hand hand);