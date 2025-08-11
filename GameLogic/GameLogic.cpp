#include "GameLogic.hpp"

static const std::string LOG_TAG = "GameLogic";

GameLogic::GameLogic(int numPlayers) {
    logDebug(LOG_TAG, "Setting up GameLogic dependencies.");
    m_dealer = std::make_unique<Dealer>();

    m_variables = std::make_shared<CommonVariables>();

    m_board = new Card[5];

    for (int i = 0; i < numPlayers; i++) {
        m_players.push_back(new Player(i, m_variables));
    }
    m_numPlayers = numPlayers;

    m_maxPot = startingStack * m_numPlayers;

    m_buttonPlayer = m_players[0];

    logDebug(LOG_TAG, "Starting game.");
    startGame();
}

GameLogic::~GameLogic() {
    delete[] m_board;

    for (auto player : m_players) {
        delete player;
    }
}

void GameLogic::setupLinkedList() {
    /* As the game goes, players will be eliminated.
       Need to determine who is in to setup turn order */
    int firstPlayerIndex = 0;
    int lastPlayerIndex = 0;

    // Find first player still in.
    for (int i = 0; i < m_numPlayers - 1; i++) {
        if (m_players[i]->m_stack > 0) {
            firstPlayerIndex = i;
            break;
        }
    }

    // Keep track of players
    m_playersInHand = 1;

    // Setup linked list.
    for (int i = firstPlayerIndex; i < m_numPlayers - 1; i++) {
        if (m_players[i + 1]->m_stack > 0) {
            m_playersInHand++;
            m_players[i]->m_nextPlayer = m_players[i + 1];
            lastPlayerIndex = i + 1;
        }
    }

    // Loop back around!
    m_players[lastPlayerIndex]->m_nextPlayer = m_players[firstPlayerIndex];
}

void GameLogic::startGame() {
    // Linked list for turn order - players involved in a hand
    setupLinkedList();
    logDebug(LOG_TAG, std::format("Set up linked list! {} players in hand.", m_playersInHand));

    dealPlayerHands();
    logDebug(LOG_TAG, "Dealt players' hands.");

    printPlayers();

    // pre-flop betting
    logDebug(LOG_TAG, "Starting pre-flop betting");
    startPreFlopBetting();

    printPot();
    printPlayers();

    dealFlop();
    printBoard();
    startPostFlopBetting();

    dealTurn();
    printBoard();
    startPostFlopBetting();

    dealRiver();
    printBoard();
    startPostFlopBetting();

    getResults();

    printPot();
    printPlayers();
}

void GameLogic::dealPlayerHands() {
    for (int i = 0; i < m_numPlayers; i++) {
        m_players[i]->m_playerHand = m_dealer->dealPlayerHand();
    }
}

void GameLogic::dealFlop() {
    m_board[0] = m_dealer->dealCard();
    m_board[1] = m_dealer->dealCard();
    m_board[2] = m_dealer->dealCard();
    m_cardsDealt = 3;
}

void GameLogic::dealTurn() {
    m_board[3] = m_dealer->dealCard();
    m_cardsDealt = 4;
}

void GameLogic::dealRiver() {
    m_board[4] = m_dealer->dealCard();
    m_cardsDealt = 5;
}

void GameLogic::startPreFlopBetting() {
    /* Have a vector of pots in case someone goes all in and a side pot is needed.
       Initial pot has no limit and no players involved in it. */
    m_pot.push_back({0, m_maxPot, std::vector<Player *>{}});

    Player *smallBlindPlayer = m_buttonPlayer->m_nextPlayer;
    Player *bigBlindPlayer = smallBlindPlayer->m_nextPlayer;

    takeBlind(m_smallBlind, smallBlindPlayer);
    takeBlind(m_bigBlind, bigBlindPlayer);
    m_variables->minimumRaiseAmount = m_bigBlind;

    unsigned int maxContribution = m_bigBlind;

    Player *currentBettingPlayer = m_buttonPlayer->m_nextPlayer->m_nextPlayer->m_nextPlayer;
    m_playersActionComplete = 0;

    startBettingRound(currentBettingPlayer, maxContribution);
    cleanUpBettingRound();
}

void GameLogic::startPostFlopBetting() {
    // There's no point in starting a betting round if there's one or 0 players left.
    if (m_playersInHand <= 1) {
        return;
    }

    Player *currentBettingPlayer = m_buttonPlayer->m_nextPlayer;
    while (currentBettingPlayer != m_buttonPlayer) {
        if (currentBettingPlayer->m_isFolded || currentBettingPlayer->m_stack == 0) {
            currentBettingPlayer = currentBettingPlayer->m_nextPlayer;
            continue;
        }
        break;
    }

    m_variables->minimumRaiseAmount = 0;
    m_playersActionComplete = 0;
    unsigned int maxContribution = 0;

    startBettingRound(currentBettingPlayer, maxContribution);
    cleanUpBettingRound();
}

void GameLogic::startBettingRound(Player *currentBettingPlayer, int maxContribution) {
    while (m_playersActionComplete != m_playersInHand) {
        if(currentBettingPlayer->m_isFolded) {
            currentBettingPlayer = currentBettingPlayer->m_nextPlayer;
            continue;
        }
        maxContribution = takeBet(maxContribution, currentBettingPlayer);
        currentBettingPlayer = currentBettingPlayer->m_nextPlayer;
    }
}

void GameLogic::cleanUpBettingRound() {
    Player* iter = m_buttonPlayer;
    do {
        iter->m_contributionToBettingRound = 0;
        iter = iter->m_nextPlayer;
    } while (iter != m_buttonPlayer);
}

void GameLogic::takeBlind(unsigned int blindAmount, Player *player) {
    // Returned value determines whether a new side pot needs to be made.
    if (player->takeBlind(blindAmount) == ActionType::AllIn) {
        updateSidePots(player);
    }
    recalculatePot();
}

unsigned int GameLogic::takeBet(unsigned int maxContribution, Player *player) {
    int amountToCall = maxContribution - player->m_contributionToBettingRound;
    ActionType action = player->takeBet(amountToCall);
    switch(action) {
        case ActionType::Call:
        case ActionType::Check:
            m_playersActionComplete++;
            break;
        case ActionType::Fold:
            m_playersInHand--;
            break;
        case ActionType::Raise:
            m_playersActionComplete = 1;
            /* If a player made a valid raise, this will be the new max contribution. */
            maxContribution = player->m_contributionToBettingRound;
            break;
        default:
            logError(LOG_TAG, "Invalid action.");
            return 0;
    }

    if (player->m_stack == 0) {
        m_playersInHand--;
        m_playersActionComplete--;
        updateSidePots(player);
    }
    recalculatePot();

    return maxContribution;
}

void GameLogic::recalculatePot() {
   /* Figuring out pot values when there are a bunch of side pots
      is kind of a pain. There are a lot of edge cases. It is much
      easier to just recalculate the pots after each bet. */
    unsigned int lastMaxPotValue = 0;

    for (auto it = m_pot.begin(); it != m_pot.end(); it++) {
        it->amount = 0;
        it->players = std::vector<Player *>{};
        for (int i = 0; i < m_numPlayers; i++) {
            bool contributedToPot = false;
            if (m_players[i]->m_contributionToCurrentHand >= it->maxBet) {
                it->amount += it->maxBet - lastMaxPotValue;
                contributedToPot = true;
            } else if (m_players[i]->m_contributionToCurrentHand < it->maxBet && m_players[i]->m_contributionToCurrentHand > lastMaxPotValue) {
                it->amount += m_players[i]->m_contributionToCurrentHand - lastMaxPotValue;
                contributedToPot = true;
            }

            if (!m_players[i]->m_isFolded && contributedToPot) {
                it->players.push_back(m_players[i]);
            }
        }
        lastMaxPotValue = it->maxBet;

        if (it->amount == 0) {
            m_pot.erase(it);
        }
    }
}

void GameLogic::updateSidePots(Player *player) {
    // The new side pot max bet will equal the contribution of the player that went all in.
    unsigned int maxBetNewSidePot = player->m_contributionToCurrentHand;

    // Pots are ordered by size of max bet
    for (auto it = m_pot.begin(); it != m_pot.end(); it++) {
        if (it->maxBet == maxBetNewSidePot) {
            // We already have a side pot of this size
            break;
        } else if (it->maxBet > maxBetNewSidePot) {
            // Make a new pot, amount and players will be calculated after
            Pot newPot = {0, maxBetNewSidePot, };
            m_pot.insert(it, newPot);
            break;
        }
    }
}

void GameLogic::getResults() {
    for (auto potIter : m_pot) {
        logInfo(LOG_TAG, std::format("Calculating pot with max bet {}.", potIter.maxBet));
        int numPlayers = potIter.players.size();
        int playerIndices[numPlayers];
        Hand playerHands[numPlayers];
        for (int i = 0; i < numPlayers; i++) {
            playerIndices[i] = potIter.players[i]->m_playerIndex;
            playerHands[i] = potIter.players[i]->m_playerHand;
        }

        std::vector<int> winningPlayers = calculateBestHand(playerIndices, playerHands, numPlayers, m_board);
        int numWinners = winningPlayers.size();
        unsigned int winningsPerPlayer = potIter.amount / numWinners;
        unsigned int remainingChips = potIter.amount % numWinners;
        for (int i = 0; i < numWinners; i++) {
            logInfo(LOG_TAG, std::format("Player {} won {} chips.", winningPlayers[i], winningsPerPlayer));
            m_players[winningPlayers[i]]->m_stack += winningsPerPlayer;
        }

        if (remainingChips != 0) {
            int playerLeftOfDealerIndex = m_buttonPlayer->m_nextPlayer->m_playerIndex;
            int playerToGetChips;
            // Make this value larger than it can be, so it will get overwritten for sure.
            int lowestDistanceFromDealer = m_playersInHand + 1;
            // Find where the remaining chips go.
            for (auto playerIter : potIter.players) {
                int playerDistanceFromDealer;
                if (playerIter->m_playerIndex > playerLeftOfDealerIndex) {
                    playerDistanceFromDealer = playerIter->m_playerIndex - playerLeftOfDealerIndex;
                } else {
                    playerDistanceFromDealer = playerLeftOfDealerIndex - playerLeftOfDealerIndex + playerIter->m_playerIndex;
                }

                if (playerDistanceFromDealer < lowestDistanceFromDealer) {
                    playerToGetChips = playerIter->m_playerIndex;
                }
            }
            logInfo(LOG_TAG, std::format("Extra {} chips went to {}.", remainingChips, playerToGetChips));
            m_players[playerToGetChips]->m_stack += remainingChips;
        }
    }
}

void GameLogic::printPlayers() {
    for (auto player : m_players) {
        logInfo(LOG_TAG, std::format("{}'s stack: {} {}'s hand: {}",  
                                     player->m_playerName, player->m_stack, 
                                     player->m_playerName, toStrHand(player->m_playerHand)));
    }
}

void GameLogic::printPot() {
    for (auto pots : m_pot) {
        std::string playerString;
        for (auto players : pots.players) {
            playerString += players->m_playerName;
            playerString += " ";
        }
        logInfo(LOG_TAG, std::format("Max bet: {} Amount: {}, Players Included: {}",
                                     pots.maxBet, pots.amount, playerString));
    }
}

void GameLogic::printBoard() {
    for (int i = 0; i < 5; i++) {
        if (i < m_cardsDealt) {
            logInfo(LOG_TAG, std::format("Board card #{}: {}", i, toStrCard(m_board[i])));
        }
    }
}
