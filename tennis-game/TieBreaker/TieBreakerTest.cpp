#include <gtest/gtest.h>
#include "TieBreaker.h"
#include "../Player/Player.h"
#include "../GameState/GameState.h"
#include "../PinInterface/PinInterface.h"
#include "../PinState/PinState.h"
#include "../ScoreBoard/ScoreBoard.h"
#include "../GameObject/GameObject.h"

class TieBreakerTest : public ::testing::Test {
protected:
    Player* _player1;
    Player* _player2;
    GameState* _gameState;
    PinInterface* _pinInterface;
    TieBreaker* _tieBreaker;
    ScoreBoard* _scoreBoard;
    GameObject* _gameObject;

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
        _scoreBoard = new ScoreBoard(  _player1, _player2, _gameState );
    }

    void TearDown() override {
        delete _tieBreaker;
        delete _pinInterface;
        delete _player2;
        delete _player1;
        delete _gameState;
        delete _scoreBoard; 
        // delete _gameObject;
    }
};

TEST_F(TieBreakerTest, SetUpSetTieBreakScenario) {
    _scoreBoard->clearScreen();
    _player1->setSetHistory(1, 6);
    _player2->setSetHistory(1, 4);
    _player1->setSetHistory(2, 5);
    _player2->setSetHistory(2, 5);
    _gameState->setCurrentSet(2);
    _player1->setGames(5);
    _player2->setGames(5);
    _player1->setPoints(2);
    _player2->setPoints(3);
    
    std::cout << "Updating scoreboard..." << std::endl;
    _scoreBoard->update();
    std::cout << "Done updating scoreboard." << std::endl;

    // Simulate Player 2 winning the next point
    _player2->setPoints(4);
    std::cout << "Player 2 scores the next point..." << std::endl;
    _scoreBoard->update();
    
    // Check if Player 2 wins the set and if the tie-breaker is ready
    if (_player2->getPoints() >= 4 && (_player2->getPoints() - _player1->getPoints()) >= 2) {
        _player2->setGames(_player2->getGames() + 1);
        EXPECT_EQ(_player2->getGames(), 6);  // Player 2 should now have 6 games
    }

    // Update and check the tie-breaker initiation
    _tieBreaker->setScoreBoards( _scoreBoard );
    _tieBreaker->initializeTieBreakMode();
    EXPECT_EQ(_tieBreaker->getIteration(), 1);
    EXPECT_EQ(_player1->getPoints(), 0);
    EXPECT_EQ(_player2->getPoints(), 0);
    EXPECT_EQ(_gameState->getServeSwitch(), 1);
    EXPECT_EQ(_gameState->getServe(), 0);
    EXPECT_EQ(_gameState->getTieLEDsOn(), 1);
    std::cout << "Tie-breaker mode initialized." << std::endl;
}
