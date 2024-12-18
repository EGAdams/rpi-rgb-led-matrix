#ifndef GAME_MODES_h
#define GAME_MODES_h
class Mode1Functions; // Add this line at the top of the file
#include "../Arduino/Arduino.h"
#include "../GameLeds/GameLeds.h"
#include "../GameState/GameState.h"
#include "../GameTimer/GameTimer.h"
#include "../Logger/Logger.h"
#include "../Mode1Functions/Mode1Functions.h"
#include "../TieBreaker/TieBreaker.h"
#include "../Mode2Functions/Mode2Functions.h"
#include "../PointLeds/PointLeds.h"
#include "../PinInterface/PinInterface.h"
#include "../Player/Player.h"
#include "../ServeLeds/ServeLeds.h"
#include "../SetLeds/SetLeds.h"
#include "../Undo/Undo.h"
#include "../Inputs/Inputs.h"
#include "../WatchTimer/WatchTimer.h"
#include "../BatteryTest/BatteryTest.h"

class History;
class TieBreaker;

#if defined _WIN32 || defined _WIN64
    #include <iostream>
#endif
class GameModes {
 public:
  GameModes( Player*       player1,
             Player*       player2,
             PinInterface*  pinInterface,
             GameState*     gameState,
             History*       history );
  ~GameModes();
  void undo();
  void gameStart();
  void testStart();
  void mode1();
  void mode2();
  void mode4();
  void noCode();
  void runGameMode(int rotaryPosition);
  void setScoreBoards( ScoreBoard* scoreBoard );

 private:
  Player* _player1;
  Player* _player2;
  PinInterface* _pinInterface;
  GameState* _gameState;
  History* _history;
  PointLeds _pointLeds;
  GameLeds _gameLeds;
  SetLeds _setLeds;
  Inputs* _inputs;
  Undo _undo;
  ServeLeds _serveLeds;
  TieBreaker* _tieBreaker;
  Mode1Functions* _mode1Functions;
  Mode2Functions _mode2Functions;
  Logger* _logger;
};

#endif
