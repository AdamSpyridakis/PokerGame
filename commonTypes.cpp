#include "commonTypes.hpp"
#include <string>

std::string toStrSuit(Suit suit) {
    switch (suit) {
        case (Diamonds):
            return "Diamonds";
        case (Hearts):
            return "Hearts";
        case (Clubs):
            return "Clubs";
        case (Spades):
            return "Spades";
    };
}

std::string toStrValue(Value value) {
    switch (value) {
        case (Ace):
            return "Ace";
        case (Two):
            return "Two";
        case (Three):
            return "Three";
        case (Four):
            return "Four";
        case (Five):
            return "Five";
        case (Six):
            return "Six";
        case (Seven):
            return "Seven";
        case (Eight):
            return "Eight";
        case (Nine):
            return "Nine";
        case (Ten):
            return "Ten";
        case (Jack):
            return "Jack";
        case (Queen):
            return "Queen";
        case (King):
            return "King";
    };
}

std::string toStrCard(Card card) {
    return toStrValue(card.value) + " of " + toStrSuit(card.suit);
}