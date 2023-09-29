#ifndef TieBreaker_h
#define TieBreaker_h
#include "../Arduino/Arduino.h"
#include "../GameLeds/GameLeds.h"
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
  TieBreaker( Player* player1,
                   Player* player2,
                   PinInterface* pinInterface,
                   GameState* gameState,
                   History* history );
  ~TieBreaker();
  void setScoreBoards( ScoreBoard* scoreBoard );
  void run( Player* currentPlayer );
  void setTieBreaker();
  void buttonAction();
  void mode1SetTBButtonFunction();
  void initializeTieBreakMode();
  void setTieBreakEnable();
  void tieLEDsOn();
  void tieLEDsOff();
  void endTieBreak();
  void mode1TBP1Games();
  void mode1TBP2Games();
  void mode1SetTBP2Games();
  void mode1SetTBP1Games();
  void celebrate();
  void incrementSet();
  void setIteration( int iteration );
  int  getIteration();
  void incrementIteration();

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
