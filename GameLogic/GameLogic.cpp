#include "GameLogic.hpp"

GameLogic::GameLogic(int numPlayers) {
    m_dealer = new Dealer;

    m_players = new Player[numPlayers];
    m_numPlayers = numPlayers;

    startGame();
}

GameLogic::~GameLogic() {
    delete m_dealer;

    delete[] m_players;
}

void GameLogic::startGame() {
    for (int i = 0; i < m_numPlayers; ++i) {
        m_dealer->dealPlayerHand(&m_players[i]);
    }
}