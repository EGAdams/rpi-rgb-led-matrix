#ifndef GAME_LEDs_HEADER_H
#define GAME_LEDs_HEADER_H

#include "../PinInterface/PinInterface.h"
#include "../Player/Player.h"
#include "../Arduino/Arduino.h"
#include "../TennisConstants/TennisConstants.h"

class GameLeds {
 public:
  GameLeds(Player* player1, Player* player2, PinInterface* pinInterface);
  ~GameLeds();
  void updateGames();

 private:
  Player* _player1;
  Player* _player2;
  PinInterface* _pinInterface;
};

#endif