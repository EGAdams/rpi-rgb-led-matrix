#ifndef TieBreaker_h
#define TieBreaker_h
#include "../Arduino/Arduino.h"
#include "../GameLeds/GameLeds.h"
#include "../GameTimer/GameTimer.h"
#include "../PointLeds/PointLeds.h"
#include "../PinInterface/PinInterface.h"
#include "../Player/Player.h"
#include "../ServeLeds/ServeLeds.h"
#include "../SetLeds/SetLeds.h"
#include "../WatchTimer/WatchTimer.h"
#include "../WinSequences/WinSequences.h"
#include "../Undo/Undo.h"
#include "../Inputs/Inputs.h"

class TieBreaker {
 public:
  TieBreaker(Player* player1, Player* player2, GameState* gameState);
  void setIteration(int iteration);
  int getIteration() const;
  void setScoreBoards(ScoreBoard* scoreBoard);
  ScoreBoard* getScoreBoards() const;
  void updateTieBreakerScore(Player* currentPlayer);

 private:
  int  _getServe();    // determine serve based on iteration
  Player* _player1;
  Player* _player2;
  PinInterface* _pinInterface;
  GameState* _gameState;
  History* _history;
  PointLeds _pointLeds;
  GameLeds _gameLeds;
  ServeLeds _serveLeds;
  SetLeds _setLeds;
  Mode1WinSequences _mode1WinSequences;
  Undo _undo;
  ScoreBoard* _scoreBoard;
  int _iteration;
};

#endif
