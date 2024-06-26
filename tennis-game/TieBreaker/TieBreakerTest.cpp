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
        _tieBreaker = new TieBreaker(_player1, _player2, _pinInterface, _gameState, NULL);
    }

    void TearDown() override {
        delete _tieBreaker;
        delete _pinInterface;
        delete _player2;
        delete _player1;
        delete _gameState;
    }
};


TEST_F(TieBreakerTest, InitializeTieBreakMode) {
    _tieBreaker->initializeTieBreakMode();
    EXPECT_EQ(_tieBreaker->getIteration(), 1);
    EXPECT_EQ(_player1->getPoints(), 0);
    EXPECT_EQ(_player2->getPoints(), 0);
    EXPECT_EQ(_gameState->getTieLEDsOn(), 1);
}

TEST_F( TieBreakerTest, ServeLogic ) {
    _tieBreaker->setIteration( 1 );
    EXPECT_EQ( _gameState->getServe(), PLAYER_2_SERVE );

    _tieBreaker->setIteration( 3 );
    EXPECT_EQ( _gameState->getServe(), PLAYER_1_SERVE );

    _tieBreaker->setIteration( 5 );
    EXPECT_EQ( _gameState->getServe(), PLAYER_2_SERVE );
}


TEST_F(TieBreakerTest, IncrementIteration) {
    _tieBreaker->setIteration(1);
    _tieBreaker->incrementIteration();
    EXPECT_EQ(_tieBreaker->getIteration(), 2);
}

TEST_F(TieBreakerTest, RunWinningScenario) {
    _player1->setPoints(6);
    _player2->setPoints(5);

    _tieBreaker->run(_player1);
    EXPECT_EQ(_player1->getGames(), 1);
    EXPECT_EQ(_gameState->getCurrentSet(), 2);
}

TEST_F(TieBreakerTest, RunContinueScenario) {
    _player1->setPoints(6);
    _player2->setPoints(6);

    _tieBreaker->run(_player1);
    EXPECT_EQ(_player1->getPoints(), 7);
    EXPECT_EQ(_gameState->getServe(), PLAYER_1_SERVE);
}

TEST_F(TieBreakerTest, TieLEDs) {
    _tieBreaker->tieLEDsOn();
    EXPECT_EQ(_gameState->getTieLEDsOn(), 1);

    _tieBreaker->tieLEDsOff();
    EXPECT_EQ(_gameState->getTieLEDsOn(), 0);
}

TEST_F(TieBreakerTest, UndoFunctionality) {
    _tieBreaker->run(_player1);
    // Assuming there are methods to check the undo state
    // Example:
    // EXPECT_TRUE(_undo->hasSnapshot());
}

TEST_F(TieBreakerTest, InitializeTieBreakModeOriginal) {
    _tieBreaker->initializeTieBreakMode();
    EXPECT_EQ(_tieBreaker->getIteration(), 1);
    EXPECT_EQ(_player1->getPoints(), 0);
    EXPECT_EQ(_player2->getPoints(), 0);
    EXPECT_EQ(_gameState->getServeSwitch(), 1);
    EXPECT_EQ(_gameState->getServe(), 0);
}

TEST_F(TieBreakerTest, ServeSequence) {
    _tieBreaker->setIteration(1);
    _tieBreaker->run(_player1);
    EXPECT_EQ(_gameState->getServe(), PLAYER_2_SERVE);

    _tieBreaker->incrementIteration();
    _tieBreaker->run(_player1);
    EXPECT_EQ(_gameState->getServe(), PLAYER_2_SERVE);

    _tieBreaker->incrementIteration();
    _tieBreaker->run(_player1);
    EXPECT_EQ(_gameState->getServe(), PLAYER_1_SERVE);

    _tieBreaker->incrementIteration();
    _tieBreaker->run(_player1);
    EXPECT_EQ(_gameState->getServe(), PLAYER_1_SERVE);

    _tieBreaker->incrementIteration();
    _tieBreaker->run(_player1);
    EXPECT_EQ(_gameState->getServe(), PLAYER_2_SERVE);
}

TEST_F(TieBreakerTest, Player1WinsTieBreaker) {
    _gameState->setTieBreak(1);
    _player1->setPoints(TIE_BREAK_MAX_POINTS);
    _tieBreaker->run(_player1);
    EXPECT_EQ(_player1->getGames(), 1);
    EXPECT_EQ(_player1->getPoints(), 0);
    EXPECT_EQ(_player2->getPoints(), 0);
}

TEST_F(TieBreakerTest, Player2WinsTieBreaker) {
    _gameState->setTieBreak(1);
    _player2->setPoints(TIE_BREAK_MAX_POINTS);
    _tieBreaker->run(_player2);
    EXPECT_EQ(_player2->getGames(), 1);
    EXPECT_EQ(_player1->getPoints(), 0);
    EXPECT_EQ(_player2->getPoints(), 0);
}

TEST_F(TieBreakerTest, Player1WinsByTwoPoints) {
    _gameState->setTieBreak(1);
    _player1->setPoints(TIE_BREAK_WIN_BY_TWO);
    _player2->setPoints(TIE_BREAK_WIN_BY_TWO - 2);
    _tieBreaker->run(_player1);
    EXPECT_EQ(_player1->getGames(), 1);
    EXPECT_EQ(_player1->getPoints(), 0);
    EXPECT_EQ(_player2->getPoints(), 0);
}

TEST_F(TieBreakerTest, Player2WinsByTwoPoints) {
    _gameState->setTieBreak(1);
    _player2->setPoints(TIE_BREAK_WIN_BY_TWO);
    _player1->setPoints(TIE_BREAK_WIN_BY_TWO - 2);
    _tieBreaker->run(_player2);
    EXPECT_EQ(_player2->getGames(), 1);
    EXPECT_EQ(_player1->getPoints(), 0);
    EXPECT_EQ(_player2->getPoints(), 0);
}

TEST_F(TieBreakerTest, EndTieBreak) {
    _tieBreaker->endTieBreak();
    EXPECT_EQ(_tieBreaker->getIteration(), 0);
    EXPECT_EQ(_player1->getPoints(), 0);
    EXPECT_EQ(_player2->getPoints(), 0);
    EXPECT_EQ(_player1->getGames(), 0);
    EXPECT_EQ(_player2->getGames(), 0);
    EXPECT_EQ(_gameState->getTieBreak(), 0);
    EXPECT_EQ(_gameState->getSetTieBreak(), 0);
    EXPECT_EQ(_gameState->getServeSwitch(), 1);
    EXPECT_EQ(_gameState->getServe(), 0);
}

TEST_F(TieBreakerTest, Player1WinsSetTieBreaker) {
    _gameState->setSetTieBreak(1);
    _player1->setGames(6);
    _player2->setGames(6);
    _tieBreaker->mode1SetTBP1Games();
    EXPECT_EQ(_player1->getSets(), 1);
}

TEST_F(TieBreakerTest, Player2WinsSetTieBreaker) {
    _gameState->setSetTieBreak(1);
    _player1->setGames(6);
    _player2->setGames(6);
    _tieBreaker->mode1SetTBP2Games();
    EXPECT_EQ(_player2->getSets(), 1);
}
