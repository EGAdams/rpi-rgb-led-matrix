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
#include "../ScoreBoard/ScoreBoard.h"

class TieBreaker {
 public:
  TieBreaker(Player* player1, Player* player2, PinInterface* pinInterface, GameState* gameState, History* history);
  ~TieBreaker();
  void setIteration(int iteration);
  int getIteration();
  void incrementIteration();
  void setScoreBoards(ScoreBoard* scoreBoard);
  void tieLEDsOn();
  void tieLEDsOff();
  void celebrate(Player* currentPlayer);
  void incrementSet();
  void run(Player* currentPlayer);
  void mode1SetTBButtonFunction();
  void setTieBreaker();
  void initializeTieBreakMode();
  void blinkSetScores();
  void setTieBreakEnable();
  void endTieBreak();
  void mode1TBP1Games();
  void mode1TBP2Games();
  void mode1SetTBP2Games();
  void mode1SetTBP1Games();

 private:
  int  _getServe();    // determine serve based on iteration
  Player* _player1;
  Player* _player2;
  void _tieBreakWin( Player* currentPlayer ); // things are getting messy, break it up
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
