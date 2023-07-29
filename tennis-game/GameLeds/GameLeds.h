#ifndef GAME_LED_HEADER_H
#define GAME_LED_HEADER_H

#include "../PinInterface/PinInterface.h"
#include "../Player/Player.h"
#include "../Arduino/Arduino.h"
#include "../TennisConstants/TennisConstants.h"
#include "../ScoreBoard/ScoreBoard.h"
#include "../GameLedTranslator/GameLedTranslator.h"

class GameLeds {
 public:
  GameLeds( IPlayer* player1, IPlayer* player2, PinInterface* pinInterface );
  ~GameLeds();
  void updateGames();
  void setScoreBoard( ScoreBoard* scoreBoard );

 private:
  IPlayer*            _player1;
  IPlayer*            _player2;
  PinInterface*      _pinInterface;
  ScoreBoard*        _scoreBoard;
  GameLedTranslator* _gameLedTranslator;
};

#endif