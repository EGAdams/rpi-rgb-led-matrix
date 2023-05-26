#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "../Arduino/Arduino.h"
#include "../GameModes/GameModes.h"
#include "../GameState/GameState.h"
#include "../GameTimer/GameTimer.h"
#include "../History/History.h"
#include "../Inputs/Inputs.h"
#include "../Logger/Logger.h"
#include "../PinInterface/PinInterface.h"
#include "../PinState/PinState.h"
#include "../Player/Player.h"
#include "../ScoreBoard/ScoreBoard.h"
#include "../SubjectManager/SubjectManager.h"
#include "../WebLiquidCrystal/WebLiquidCrystal.h"
#include <map>

class GameObject {
 public:
  GameObject(Player* player1,
             Player* player2,
             PinState* pinState,
             PinInterface* pinInterface,
             GameState* gameState,
             GameTimer* gameTimer,
             Inputs* gameInputs,
             GameModes* gameModes,
             ScoreBoard* scoreBoard,
             WebLiquidCrystal* lcd);
  GameObject();
  ~GameObject();
  void start();
  PinInterface* getPinInterface();
  void loopGame();

 private:
  Player* _player1;
  Player* _player2;
  PinState* _pinState;
  PinInterface* _pinInterface;
  GameState* _gameState;
  GameTimer* _gameTimer;
  Inputs* _gameInputs;
  GameModes* _gameModes;
  ScoreBoard* _scoreBoard;
  WebLiquidCrystal* _webLiquidCrystal;
  SubjectManager* _subjectManager;
  Logger* _logger;
  History* _history;
#if defined _WIN32 || defined _WIN64
  std::map<std::string, int> _pin_map;
#endif
};

#endif
