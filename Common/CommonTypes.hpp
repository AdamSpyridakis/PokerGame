#pragma once

#include <string>
#include <vector>

const int startingStack = 1000;
const int startingBigBlind = 2;
const int startingSmallBlind = 1;

struct CommonVariables
{
    int minimumRaiseAmount;
};

enum Suit {
    Diamonds,
    Hearts,
    Clubs,
    Spades
};

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

struct Card {
    Suit suit;
    Value value;
};

struct Hand {
    Card firstCard;
    Card secondCard;
};

std::string toStrSuit(Suit suit);
std::string toStrValue(Value value);
std::string toStrCard(Card card);
std::string toStrHand(Hand hand);