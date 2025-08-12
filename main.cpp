#include "GameLogic/GameLogic.hpp"
#include "Common/CommonTypes.hpp"
#include "GameLogic/HandCalculator.hpp"
#include "Dealer/Dealer.hpp"
#include "Common/Logging.hpp"

int main() {
    logEssential("Enter number of players.");
    int numPlayers;
    std::cin >> numPlayers;
    GameLogic Test(numPlayers);
    return 0;
}
