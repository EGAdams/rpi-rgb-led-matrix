#include <gtest/gtest.h>
#include "TieBreaker.h"

class TieBreakerTest : public ::testing::Test {
protected:
    Player* _player1;
    Player* _player2;
    PinInterface* _pinInterface;
    GameState* _gameState;
    History* _history;
    ScoreBoard* _scoreBoard;
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
        _history = new History();
        _scoreBoard = new ScoreBoard();
        _tieBreaker = new TieBreaker(_player1, _player2, _pinInterface, _gameState, _history);
        _tieBreaker->setScoreBoards(_scoreBoard);
    }

    void TearDown() override {
        delete _tieBreaker;
        delete _scoreBoard;
        delete _history;
        delete _gameState;
        delete _pinInterface;
        delete _player2;
        delete _player1;
    }
};
