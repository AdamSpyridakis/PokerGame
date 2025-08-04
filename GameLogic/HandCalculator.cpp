#include "HandCalculator.hpp"

static const std::string LOG_TAG = "Hand Calculator";

int calculateBestHand(int playerIndex[], Hand playerHand[], int size, Card board[]) {
    BestHand classifiedHands[size];
    for (int i = 0; i < size; i++) {
        classifiedHands[i] = classifyHand(playerHand[i], board);
    }

    return 0;
}

BestHand classifyHand(Hand playerHand, Card board[]) {
    BestHand bestHand = {{}, HighCard};

    // Set up an array of available cards, easier to parse.
    Card cardsAvailable[7] = {playerHand.firstCard, playerHand.secondCard,
                              board[0], board[1], board[2], board[3], board[4]};    

    // Start by gathering information on suit.
    int suit = sortBySuit(cardsAvailable);
    if (suit != -1) {
        bestHand = checkForStraightFlush(static_cast<Suit>(suit), cardsAvailable);
        switch (bestHand.handStrength) {
            case RoyalFlush:
                return bestHand;
            case StraightFlush:
                return bestHand;
            // Need to check four of a kind and full house first.
            case Flush:
                break;
            default:
                logError(LOG_TAG, "Unexpected state in switch case.");
                break;
        }
    }

    // Gather information on value.
    int mostDuplicates = sortByDuplicate(cardsAvailable);
    if (mostDuplicates == 4) {
        bestHand.handStrength = FourOfAKind;
        std::copy(cardsAvailable, cardsAvailable + 4, bestHand.hand);
        std::set<Value> excludedValues = {cardsAvailable[0].value};
        findKicker(1, excludedValues, cardsAvailable, bestHand.hand);
        return bestHand;
    } else if (mostDuplicates == 3) {
        if (cardsAvailable[3].value == cardsAvailable[4].value) {
            bestHand.handStrength = FullHouse;
            std::copy(cardsAvailable, cardsAvailable + 5, bestHand.hand);
            return bestHand;
        }
    }

    if (bestHand.handStrength == Flush) {
        return bestHand;
    }

    sortByValue(cardsAvailable, 0, 6);
    bestHand = checkForStraight(0, 6, cardsAvailable);
    if (bestHand.handStrength == Straight) {
        return bestHand;
    }

    mostDuplicates = sortByDuplicate(cardsAvailable);
    std::set<Value> excludedValues = {};
    switch (mostDuplicates) {
        case 3:
            bestHand.handStrength = ThreeOfAKind;
            std::copy(cardsAvailable, cardsAvailable + 3, bestHand.hand);
            excludedValues.insert(cardsAvailable[0].value);
            findKicker(2, excludedValues, cardsAvailable, bestHand.hand);
            return bestHand;
        case 2:
            if (cardsAvailable[2].value == cardsAvailable[1].value) {
                bestHand.handStrength = TwoPair;
                std::copy(cardsAvailable, cardsAvailable + 4, bestHand.hand);
                excludedValues.insert(cardsAvailable[0].value);
                excludedValues.insert(cardsAvailable[2].value);
                findKicker(1, excludedValues, cardsAvailable, bestHand.hand);
            } else {
                bestHand.handStrength = Pair;
                std::copy(cardsAvailable, cardsAvailable + 2, bestHand.hand);
                excludedValues.insert(cardsAvailable[0].value);
                findKicker(3, excludedValues, cardsAvailable, bestHand.hand);
            }
            return bestHand;
        case 1:
            bestHand.handStrength = HighCard;
            findKicker(5, excludedValues, cardsAvailable, bestHand.hand);
            return bestHand;
        default:
            logError(LOG_TAG, "Unexpected state in switch case.");
            // Getting here is already detrimental, so can return whatever.
            return bestHand;
    }
}

int sortBySuit(Card cardsAvailable[]) {
    int foundFlush = -1;
    int endIndex = 0;
    int startIndex = 0;
    for (int i = Diamonds; i < Spades; i++) {
        for (int j = 0; j < 7; j++) {
            if (cardsAvailable[j].suit == i) {
                std::swap(cardsAvailable[endIndex], cardsAvailable[j]);
                endIndex++;
            }
        }
        if (endIndex - startIndex >= 5) {
            foundFlush = i;
        }
        sortByValue(cardsAvailable, startIndex, endIndex - 1);
        startIndex = endIndex;
    }
    return foundFlush;
}

int sortByDuplicate(Card cardsAvailable[]) {
    sortByValue(cardsAvailable, 0, 6);

    int maximumDuplicates = 1;
    Card tempCards[7];
    for (int i = 0; i < 7;) {
        int mostDuplicates = 1;
        int currentDuplicates = 1;
        int mostDuplicatesStartIndex = i;
        int currentDuplicatesStartIndex = i;
        for (int j = i + 1; j < 7; j++) {
            if (cardsAvailable[j].value == cardsAvailable[j - 1].value) {
                currentDuplicates++;
            } else {
                currentDuplicates = 1;
                currentDuplicatesStartIndex = j;
            }
            if (currentDuplicates > mostDuplicates) {
                mostDuplicates = currentDuplicates;
                mostDuplicatesStartIndex = currentDuplicatesStartIndex;
            }
        }

        std::copy(cardsAvailable + mostDuplicatesStartIndex, cardsAvailable + 7, tempCards + i);
        std::copy(cardsAvailable + i, cardsAvailable + mostDuplicatesStartIndex, tempCards + 7 + i - mostDuplicatesStartIndex);
        std::copy(tempCards, tempCards + 7, cardsAvailable);
        sortByValue(cardsAvailable, i + mostDuplicates, 6);

        if (i == 0) {
            maximumDuplicates = mostDuplicates;
        }
        i += mostDuplicates;
    }

    return maximumDuplicates;
}

void sortByValue(Card cardsAvailable[], int startIndex, int endIndex) {
    for (int i = startIndex; i < endIndex; i++) {
        int highestIndex = i;
        for (int j = i + 1; j < endIndex + 1; j++) {
            if (cardsAvailable[j].value >= cardsAvailable[highestIndex].value) {
                highestIndex = j;
            }
        }
        std::swap(cardsAvailable[highestIndex], cardsAvailable[i]);
    }
}

void findKicker(int numKickers, std::set<Value> valuesExcluded, Card cardsAvailable[], Card hand[]) {
    sortByValue(cardsAvailable, 0, 6);
    for (int i = 0; i < 7; i++) {
        if (valuesExcluded.contains(cardsAvailable[i].value)) {
            continue;
        } else {
            hand[5 - numKickers] = cardsAvailable[i];
            numKickers--;
        }
        if (numKickers == 0) {
            return;
        }
    }
}

BestHand checkForStraightFlush(Suit suit, Card cardsAvailable[]) {
    // cardsAvailable is already sorted, so find where the suit begins.
    int startIndex = 0;
    // startIndex SHOULD be < 3 considering there is a flush.
    for (startIndex; startIndex < 3; startIndex++) {
        if (cardsAvailable[startIndex].suit == suit) {
            break;
        }
    }

    int endIndex = startIndex + 4;
    for (endIndex; endIndex < 7; endIndex++) {
        if (cardsAvailable[endIndex + 1].suit != suit) {
            break;
        }
    }

    BestHand bestHand = checkForStraight(startIndex, endIndex, cardsAvailable);
    if (bestHand.handStrength == Straight) {
        if (bestHand.hand[0].value == Ace) {
            bestHand.handStrength = RoyalFlush;
        } else {
            bestHand.handStrength = StraightFlush;
        }
        return bestHand;
    }

    // Populate the flush hand
    bestHand.handStrength = Flush;
    std::copy(cardsAvailable + startIndex, cardsAvailable + startIndex + 5, bestHand.hand);
    return bestHand;
}

BestHand checkForStraight(int startIndex, int endIndex, Card cardsAvailable[]) {
    BestHand bestHand;
    int cardsInARow = 0;
    for (int i = startIndex; i < endIndex; i++) {
        if (cardsAvailable[i].value - cardsAvailable[i + 1].value == 1) {
            bestHand.hand[cardsInARow] = cardsAvailable[i];
            cardsInARow++;
        } else {
            cardsInARow = 0;
        }
        if (cardsInARow == 4) {
            bestHand.hand[cardsInARow] = cardsAvailable[i + 1];
            bestHand.handStrength = Straight;
            return bestHand;
        }
    }

    // Check for wheel straight
    if (cardsInARow == 3 && cardsAvailable[startIndex].value == Ace) {
        bestHand.hand[3] = cardsAvailable[endIndex];
        bestHand.hand[4] = cardsAvailable[startIndex];
        bestHand.handStrength = Straight;
        return bestHand;
    }

    bestHand.handStrength = HighCard;
    return bestHand;
}
