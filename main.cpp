#include "GameLogic/GameLogic.hpp"
#include "Common/CommonTypes.hpp"
#include "GameLogic/HandCalculator.hpp"
#include "Dealer/Dealer.hpp"

int main() {
    Dealer dealer;
    Card board[5];
    Hand hand;
    Card cardsAvailable[7];
    // for (int i = 0; i < 5; i++) {
    //     board[i] = dealer.dealCard();
    //     std::cout << toStrCard(board[i]) << "\n";
    // }
    board[0] = {Diamonds, Ace};
    board[1] = {Diamonds, King};
    board[2] = {Diamonds, Queen};
    board[3] = {Diamonds, Jack};
    board[4] = {Diamonds, Ten};
    hand = dealer.dealPlayerHand();
    std::cout << toStrCard(hand.firstCard) << "\n" << toStrCard(hand.secondCard);

    std::cout << "\n\n";

    std::copy(board, board + 5, cardsAvailable);
    cardsAvailable[5] = hand.firstCard;
    cardsAvailable[6] = hand.secondCard;

    

    BestHand bestHand = classifyHand(hand, board);
    std::cout << bestHand.handStrength;
    for (int i = 0; i < 5; i++) {
        std::cout << toStrCard(bestHand.hand[i]) << "\n";
    }

    // GameLogic Test(3);
    return 0;
}
