#include "GameLogic.hpp"

GameLogic::GameLogic(int numPlayers) {
    m_dealer = new Dealer;

    for (int i = 0; i < numPlayers; ++i) {
        m_players.push_back(new Player(i));
    }
    m_numPlayers = numPlayers;

    maxPot = startingStack * m_numPlayers;

    m_buttonPlayer = m_players[0];

    startGame();
}

GameLogic::~GameLogic() {
    delete m_dealer;

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

    // Setup linked list.
    for (int i = firstPlayerIndex; i < m_numPlayers - 1; ++i) {
        if (m_players[i + 1]->m_stack > 0) {
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

    dealPlayerHands();

    printPlayers();

    takeInitialBets();

    printPot();

    // pre-flop betting
    beginBetting();

}

void GameLogic::dealPlayerHands() {
    for (int i = 0; i < m_numPlayers; ++i) {
        m_players[i]->m_playerHand = m_dealer->dealPlayerHand();
    }
}

void GameLogic::takeInitialBets() {
    /* Have a vector of pots in case someone goes all in and a side pot is needed.
       Initial pot has no limit and no players involved in it. */
    m_pot.push_back({0, maxPot, std::vector<Player *>{}});

    Player *smallBlindPlayer = m_buttonPlayer->m_nextPlayer;
    Player *bigBlindPlayer = smallBlindPlayer->m_nextPlayer;

    updatePot(m_smallBlind, smallBlindPlayer, true);
    updatePot(m_bigBlind, bigBlindPlayer, true);
}

void GameLogic::beginBetting() {
    Player *currentBettingPlayer = m_buttonPlayer->m_nextPlayer->m_nextPlayer;

}

void GameLogic::updatePot(int betAmount, Player *player, bool isForced) {
    /* Send player betAmount (amount to call/blind amount).
       Returned value determines whether a new side pot needs to be made. */ 
    bool allIn = player->takeBets(betAmount, isForced);

    if (allIn) {
        updateSidePots(player);
    } 
    
    /* Figuring out pot values when there are a bunch of side pots
       is kind of a pain. There are a lot of edge cases. It is much
       easier to just recalculate the pots after each bet. */
    int lastMaxPotValue = 0;

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
    int maxBetNewSidePot = player->m_contributionToCurrentHand;

    // Pots are ordered by size of max bet
    for (auto it = m_pot.begin(); it != m_pot.end(); ++it) {
        if (it->maxBet == maxBetNewSidePot) {
            // We already have a side pot of this size
            break;
        } else if (it->maxBet > maxBetNewSidePot) {
            // Make a new pot, amount and players will be calculated after
            Pot newPot = {0, maxBetNewSidePot, };
            m_pot.insert(it, newPot);
        }
    }
}

void GameLogic::printPlayers() {
    for (auto player : m_players) {
        std::cout << player->m_playerName << " " << player->m_stack << "\n";
        std::cout << toStrHand(player->m_playerHand) << "\n\n";
    }
}

void GameLogic::printPot() {
    for (auto pots : m_pot) {
        std::cout << "Max bet: " << pots.maxBet << "\n";
        std::cout << "Amount: " << pots.amount << "\n";
        std::cout << "Players Included: ";
        for (auto players : pots.players) {
            std::cout << players->m_playerName << " ";
        }
        std::cout << "\n\n";
    }
}