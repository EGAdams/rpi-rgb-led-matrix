#ifndef Reset_h
#define Reset_h

#include "../GameState/GameState.h"
#include "../GameTimer/GameTimer.h"
#include "../PinInterface/PinInterface.h"
#include "../Logger/Logger.h"
#include "../Arduino/Arduino.h"
#include "../TennisConstants/TennisConstants.h"
#include "../PinInterface/PinInterface.h"

class Reset {
 public:
  Reset( IPlayer*       player1,
         IPlayer*       player2,
         PinInterface*  pinInterface,
         GameState*     gameState );
  ~Reset();
  void resetScoreboard();
  void zeroPlayerValues();
  void refresh();
  void tieLEDsOn();
  void tieLEDsOff();

 private:
  IPlayer* _player1;
  IPlayer* _player2;
  PinInterface* _pinInterface;
  GameState* _gameState;
  Logger* _logger;
};

#endif
