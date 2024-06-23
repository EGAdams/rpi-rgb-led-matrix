#ifndef TIEBREAKERTEST_H
#define TIEBREAKERTEST_H

#include <gtest/gtest.h>
#include "TieBreaker.h"
#include "Player.h"
#include "GameState.h"
#include "PinInterface.h"
#include "PinState.h"

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
        _tieBreaker = new TieBreaker(_player1, _player2, _pinInterface, _gameState);
    }

    void TearDown() override {
        delete _tieBreaker;
        delete _pinInterface;
        delete _player2;
        delete _player1;
        delete _gameState;
    }
};

#endif // TIEBREAKERTEST_H
