#include "GameLogic.hpp"

GameLogic::GameLogic(int numPlayers) {
    m_dealer = new Dealer;

    for (int i = 0; i < numPlayers; ++i) {
        m_players.push_back(new Player);
    }
    m_numPlayers = numPlayers;
    for (auto player : m_players) {
        player->setPlayerName();
    }

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

void GameLogic::startGame() {
    dealPlayerHands();

    takeInitialBets();

    // pre-flop betting
    beginBetting();

}

void GameLogic::dealPlayerHands() {
    for (int i = 0; i < m_numPlayers; ++i) {
        m_players[i]->setPlayerHand(m_dealer->dealPlayerHand());
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
    int actualBet = player->takeBets(betAmount);

    /* In the case that a side pot must be made, the expected amount will be greater than what the player put in.
       For example a player calls when the bet is more than the amount of chips they have. */
    if (betAmount > actualBet) {
        updateSidePots(betAmount, player);
    }


}

void GameLogic::updateSidePots(int betAmount, Player *player) {
    // The new side pot max bet will equal the contribution of the player that went all in.
    int maxBetNewSidePot = player->getContribution();

    // Pots are ordered by size of max bet
    for (auto it = m_pot.begin(); it != m_pot.end(); ++it) {
        if (it->maxBet == maxBetNewSidePot) {
            // We already have a side pot of this size
            break;
        } else if (it->maxBet > player->getContribution()) {
            // Make a new pot, amount will be calculated after
            Pot newPot = {0, player->getContribution(), it->players};
            m_pot.insert(it, newPot);
        }
    }
}