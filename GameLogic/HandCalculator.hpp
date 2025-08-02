#pragma once

#include "../Common/CommonTypes.hpp"
#include "../Common/Logging.hpp"

#include <algorithm>
#include <set>

enum HandClassification {
    RoyalFlush,
    StraightFlush,
    FourOfAKind,
    FullHouse,
    Flush,
    Straight,
    ThreeOfAKind,
    TwoPair,
    Pair,
    HighCard
};

struct BestHand {
    Card hand[5];
    HandClassification handStrength;
};

int calculateBestHand(int playerIndex[], Hand playerHand[], int size, Card board[]);

// Returns suit enum value if there is a flush, otherwise -1.
int sortBySuit(Card cardsAvailable[]);

// Returns maximum duplicate value.
int sortByDuplicate(Card cardsAvailable[]);

void sortByValue(Card cardsAvailable[], int startIndex, int endIndex);

void findKicker(int numKickers, std::set<Value> valuesExcluded, Card cardsAvailable[], Card hand[]);

BestHand classifyHand(Hand playerHand, Card board[]);

BestHand checkForStraightFlush(Suit suit, Card cardsAvailable[]);

BestHand checkForStraight(int startIndex, int endIndex, Card cardsAvailable[]);