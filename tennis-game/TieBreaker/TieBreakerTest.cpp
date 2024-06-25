#include <gtest/gtest.h>
#include "TieBreaker.h"
#include "../Player/Player.h"
#include "../GameState/GameState.h"
#include "../PinInterface/PinInterface.h"
#include "../PinState/PinState.h"

class TieBreakerTest : public ::testing::Test {
protected:
    Player* _player1;
    Player* _player2;
    GameState* _gameState;
    PinInterface* _pinInterface;
    TieBreaker* _tieBreaker;

    void SetUp() override {
        _gameState = new GameState();
        _player1 = new Player(_gameState, PLAYER_1_INITIALIZED);
        _player2 = new Player(_gameState, PLAYER_2_INITIALIZED);
        _player1->setOpponent(_player2);
        _player2->setOpponent(_player1);
        std::map<std::string, int> pin_map;
        PinState* pin_state = new PinState(pin_map);
        _pinInterface = new PinInterface(pin_state);
        _tieBreaker = new TieBreaker( _player1, _player2, _pinInterface, _gameState, NULL );
    }

    void TearDown() override {
        delete _tieBreaker;
        delete _pinInterface;
        delete _player2;
        delete _player1;
        delete _gameState;
    }
};


TEST_F( TieBreakerTest, Player1WinsTieBreaker ) {
    std::cout << "Set tiebreak flag" << std::endl;
    _gameState->setTieBreak( 1 );
    std::cout << "Set player 1 points to 6" << std::endl;
    _player1->setPoints( 6 );
    std::cout << "Set player 2 points to 6" << std::endl;
    _player2->setPoints( 6 );
    std::cout << "Player 1 scores, now 7-6" << std::endl;
    _tieBreaker->run( _player1 );
    std::cout << "Expect player 1 points to be 7" << std::endl;
    EXPECT_EQ( _player1->getPoints(), 7 );
    std::cout << "Expect player 2 points to be 6" << std::endl;
    EXPECT_EQ( _player2->getPoints(), 6 );

    std::cout << "Player 1 scores again, now 8-6, wins the tie-break and set" << std::endl;
    _tieBreaker->run( _player1 );
    std::cout << "Expect player 1 points to be 0" << std::endl;
    EXPECT_EQ( _player1->getPoints(), 0 );
    std::cout << "Expect player 2 points to be 0" << std::endl;
    EXPECT_EQ( _player2->getPoints(), 0 );
    std::cout << "Expect player 1 games to be 0" << std::endl;
    EXPECT_EQ( _player1->getGames(), 0 );
    std::cout << "Expect player 2 games to be 0" << std::endl;
    EXPECT_EQ( _player2->getGames(), 0 );
    std::cout << "Expect player 1 sets to be 1" << std::endl;
    EXPECT_EQ( _player1->getSets(), 1 );
}

TEST_F(TieBreakerTest, Player2WinsTieBreaker) {
    _gameState->setTieBreak(1);
    _player1->setPoints(6);
    _player2->setPoints(6);
    _tieBreaker->run(_player2);  // Player 2 scores, now 7-6
    EXPECT_EQ(_player1->getPoints(), 6);
    EXPECT_EQ(_player2->getPoints(), 7);

    _tieBreaker->run(_player2);  // Player 2 scores again, now 8-6, wins the tie-break and set
    EXPECT_EQ(_player1->getPoints(), 0);
    EXPECT_EQ(_player2->getPoints(), 0);
    EXPECT_EQ(_player1->getGames(), 0);
    EXPECT_EQ(_player2->getGames(), 0);
    EXPECT_EQ(_player2->getSets(), 1);
}
