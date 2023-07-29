#ifndef Undo_h
#define Undo_h
#include "../Arduino/Arduino.h"
#include "../GameLeds/GameLeds.h"
#include "../GameState/GameState.h"
#include "../GameTimer/GameTimer.h"
#include "../History/History.h"
#include "../Logger/Logger.h"
#include "../PointLeds/PointLeds.h"
#include "../PinInterface/PinInterface.h"
#include "../ServeLeds/ServeLeds.h"
#include "../SetLeds/SetLeds.h"
#include "../TieLeds/TieLeds.h"
#include "../TennisConstants/TennisConstants.h"

class Undo {
 public:
    Undo( IPlayer* player1, IPlayer* player2, PinInterface* pinInterface, GameState* gameState );
    ~Undo();
    void setMode1Undo( History* history );
    void memory();
    void mode1Undo( History* history );

 private:
    IPlayer*       _player1; std::map< std::string, int > _player1_set_history;
    IPlayer*       _player2; std::map< std::string, int > _player2_set_history;
    PinInterface* _pinInterface;
    GameState*    _gameState;
    PointLeds     _pointLeds;
    GameLeds      _gameLeds;
    SetLeds       _setLeds;
    ServeLeds     _serveLeds;
    TieLeds       _tieLeds;
    Logger*       _logger; };
#endif