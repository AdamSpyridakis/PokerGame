#include "GameLogic.hpp"

GameLogic::GameLogic(int numPlayers) {
    m_dealer = new Dealer;

    for (int i = 0; i < numPlayers; ++i) {
        m_players.push_back(new Player(i));
    }
    setupLinkedList();
    m_numPlayers = numPlayers;

    maxPot = startingStack * m_numPlayers;

    m_buttonPlayer = 0;

    startGame();
}

GameLogic::~GameLogic() {
    delete m_dealer;

    for (auto player : m_players) {
        delete player;
    }
}

void GameLogic::setupLinkedList() {
    for (int i = 0; i < m_numPlayers; ++i) {
    }
}

void GameLogic::startGame() {
    dealPlayerHands();

#ifdef CONSOLE_PRINT
    printPlayers();
#endif

    takeInitialBets();

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

    int smallBlindPlayer = m_buttonPlayer + 1;
    int bigBlindPlayer = m_buttonPlayer + 2;

    if (m_buttonPlayer == m_numPlayers - 1) {
        smallBlindPlayer -= m_numPlayers;
    } 

    if (smallBlindPlayer == m_numPlayers - 1) {
        bigBlindPlayer -= m_numPlayers;
    }

    updatePot(smallBlind, m_players[smallBlindPlayer]);

}

void GameLogic::beginBetting() {

}

void GameLogic::updatePot(int betAmount, Player *player) {
    bool allIn = player->takeBets(betAmount);

    if (allIn) {
        updateSidePots(player);
    } 
    
    // Recalculate pot values
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

#ifdef CONSOLE_PRINT
void GameLogic::printPlayers() {
    for (auto player : m_players) {
        std::cout << player->m_playerName << "\n";
        std::cout << toStrHand(player->m_playerHand) << "\n\n";
    }
}
#endif