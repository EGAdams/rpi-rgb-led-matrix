#ifndef MODE_1_SCORE_H
#define MODE_1_SCORE_H

#include "../Arduino/Arduino.h"
#include "../GameLeds/GameLeds.h"
#include "../GameState/GameState.h"
#include "../GameTimer/GameTimer.h"
#include "../TieBreaker/TieBreaker.h"
#include "../WinSequences/WinSequences.h"
#include "../PointLeds/PointLeds.h"
#include "../PinState/PinState.h"
#include "../PinInterface/PinInterface.h"
#include "../Player/Player.h"
#include "../SetLeds/SetLeds.h"
#include "../Undo/Undo.h"
#include "../TennisConstants/TennisConstants.h"
#include <iostream>
// #include "../../tennis-game/googletest/googletest/include/gtest/gtest.h" 

class Mode1Score {
 public:
    Mode1Score( Player* player1,
                Player* player2,
                PinInterface* pinInterface,
                GameState* gameState,
                History* history );
    ~Mode1Score();
    void playerOneScore();
    void playerOneGameWin();
    void mode1TBP1Games();
    void mode1SetTBP1Games();
    void playerTwoScore();
    void playerTwoGameWin();
    void playerGameWin( Player* currentPlayer );
    void mode1TBP2Games();
    void mode1SetTBP2Games();
    void updateScore(  Player* currentPlayer  );
    void setScoreBoard( ScoreBoard* scoreBoard );
    ScoreBoard* getScoreBoard();
    TieBreaker* getTieBreaker();

 private:
    void _resetGame();
    Player* _player1;
    Player* _player2;
    GameState* _gameState;
    History* _history;
    TieBreaker _tieBreaker;
    PointLeds _pointLeds;
    GameLeds _gameLeds;
    SetLeds _setLeds;
    Mode1WinSequences _mode1WinSequences;
    Undo _undo;
    ScoreBoard* _scoreBoard; };
#endif
