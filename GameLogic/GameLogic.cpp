#include "GameLogic.hpp"

static const std::string LOG_TAG = "GameLogic";

GameLogic::GameLogic(int numPlayers) {
    logDebug(LOG_TAG, "Setting up GameLogic dependencies.");
    m_dealer = std::make_unique<Dealer>();

    m_variables = std::make_shared<CommonVariables>();

    m_board = new Card[5];

    for (int i = 0; i < numPlayers; ++i) {
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
    for (int i = 0; i < m_numPlayers - 1; ++i) {
        if (m_players[i]->m_stack > 0) {
            firstPlayerIndex = i;
            break;
        }
    }

    // Keep track of players
    m_playersInHand = 1;

    // Setup linked list.
    for (int i = firstPlayerIndex; i < m_numPlayers - 1; ++i) {
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

    takeInitialBets();
    logDebug(LOG_TAG, "Took blinds.");

    printPot();

    // pre-flop betting
    logDebug(LOG_TAG, "Starting pre-flop betting");
    startBettingRound();

    dealFlop();

    printPot();
    printPlayers();
}

void GameLogic::dealPlayerHands() {
    for (int i = 0; i < m_numPlayers; ++i) {
        m_players[i]->m_playerHand = m_dealer->dealPlayerHand();
    }
}

void GameLogic::dealFlop() {
    m_board[0] = m_dealer->dealCard();
    m_board[1] = m_dealer->dealCard();
    m_board[2] = m_dealer->dealCard();
}

void GameLogic::takeInitialBets() {
    /* Have a vector of pots in case someone goes all in and a side pot is needed.
       Initial pot has no limit and no players involved in it. */
    m_pot.push_back({0, m_maxPot, std::vector<Player *>{}});

    Player *smallBlindPlayer = m_buttonPlayer->m_nextPlayer;
    Player *bigBlindPlayer = smallBlindPlayer->m_nextPlayer;

    takeBlind(m_smallBlind, smallBlindPlayer);
    takeBlind(m_bigBlind, bigBlindPlayer);
    m_variables->minimumRaiseAmount = m_bigBlind;
}

void GameLogic::startBettingRound() {
    // Keeps track of maximum call amount (if someone has put in nothing)
    unsigned int maximumCallAmount = m_bigBlind;

    Player *currentBettingPlayer = m_buttonPlayer->m_nextPlayer->m_nextPlayer->m_nextPlayer;
    m_playersActionComplete = 1;
    while (m_playersActionComplete != m_playersInHand) {
        if(currentBettingPlayer->m_isFolded) {
            currentBettingPlayer = currentBettingPlayer->m_nextPlayer;
            continue;
        }
        maximumCallAmount = takeBet(maximumCallAmount, currentBettingPlayer);
        currentBettingPlayer = currentBettingPlayer->m_nextPlayer;
    }
}

void GameLogic::takeBlind(unsigned int blindAmount, Player *player) {
    // Returned value determines whether a new side pot needs to be made.
    if (player->takeBlind(blindAmount) == ActionType::AllIn) {
        updateSidePots(player);
    }
    recalculatePot();
}

unsigned int GameLogic::takeBet(unsigned int maxCallAmount, Player *player) {
    int amountToCall = maxCallAmount - player->m_contributionToBettingRound;
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
            /* If a player made a valid raise, this will be the new call amount
               for other players. */
            maxCallAmount = player->m_contributionToBettingRound;
            break;
        default:
            logError(LOG_TAG, "Invalid action.");
            return 0;
    }

    if (player->m_stack == 0) {
        updateSidePots(player);
    }
    recalculatePot();

    return maxCallAmount;
}

void GameLogic::recalculatePot() {
   /* Figuring out pot values when there are a bunch of side pots
      is kind of a pain. There are a lot of edge cases. It is much
      easier to just recalculate the pots after each bet. */
    unsigned int lastMaxPotValue = 0;

    for (auto it = m_pot.begin(); it != m_pot.end(); ++it) {
        it->amount = 0;
        it->players = std::vector<Player *>{};
        for (int i = 0; i < m_numPlayers; ++i) {
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
    }
}

void GameLogic::updateSidePots(Player *player) {
    // The new side pot max bet will equal the contribution of the player that went all in.
    unsigned int maxBetNewSidePot = player->m_contributionToCurrentHand;

    // Pots are ordered by size of max bet
    for (auto it = m_pot.begin(); it != m_pot.end(); ++it) {
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