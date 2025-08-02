#include "GameLogic/GameLogic.hpp"
#include "Common/CommonTypes.hpp"
#include "GameLogic/HandCalculator.hpp"
#include "Dealer/Dealer.hpp"

int main() {
    Dealer dealer;
    Card board[5];
    Hand hand;
    Card cards[7];
    for (int i = 0; i < 7; i++) {
        cards[i] = dealer.dealCard();
        std::cout << toStrCard(cards[i]) << "\n";
    }

    std::cout << "\n\n";

    hand.firstCard = cards[0];
    hand.secondCard = cards[1];
    std::copy(cards, cards + 5, board);

    BestHand bestHand = classifyHand(hand, board);
    std::cout << bestHand.handStrength;
    for (int i = 0; i < 5; i++) {
        std::cout << toStrCard(bestHand.hand[i]) << "\n";
    }

    // GameLogic Test(3);
    return 0;
}
