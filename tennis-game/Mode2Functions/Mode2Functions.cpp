#include "Mode2Functions.h"

Mode2Functions::Mode2Functions(Player*      player1,
                               Player*      player2,
                               PinInterface* pinInterface,
                               GameState*    gameState)
    : _player1(player1),
      _player2(player2),
      _gameState(gameState),
      _pointLeds(player1, player2, pinInterface),
      _gameLeds(player1, player2, pinInterface),
      _setLeds(player1, player2, pinInterface){};
Mode2Functions::~Mode2Functions() {}

void Mode2Functions::m2Loop() {
  _gameState->setNow(/* now = */ GameTimer::gameMillis());
  if (_gameState->getNow() - _gameState->getPreviousTime()  >= 1000 ) {
    switch (_player1->getMode()) { /* modeP1 */

      // _player1 Points
      case 0:
        _player1->incrementSetting();
        _player1->setPoints(_player1->getSetting());
        std::cout << "inside m2Loop...  player1 points: " << _player1->getPoints() << std::endl;
        _pointLeds.updatePoints();
        if ( _player1->getSetting() > 3 ) {
          _player1->setSetting( 0 );
          _player1->setPoints( 0 );
          _player1->setMode( 1 ); }
        break;

      // _player1 Games
      case 1:
        _player1->incrementSetting();
        _player1->setGames(_player1->getSetting());
        _gameLeds.updateGames();
        if (_player1->getSetting() > 6 ) {
          _player1->setSetting( 0 );
          _player1->setGames( 0 );
          _player1->setMode( 2 ); }
        break;

      // _player1 Sets
      case 2:
        _player1->incrementSetting();
        _player1->setSets( _gameState, _player1->getSetting());
        _setLeds.updateSets();                      
        if (_player1->getSetting() > 1 ) {
          _player1->setSetting( 0 );  
          _player1->setSets( _gameState, 0 );
          _player1->setMode( 0 );     
        }
        break; }

    // _player2 Points
    switch (_player2->getMode() ) {
      case 0:
        _player2->setPoints(
        _player2->incrementSetting());
        std::cout << "inside m2Loop...  player2 points: " << _player2->getPoints() << std::endl;
        _pointLeds.updatePoints();         
        if (_player2->getSetting() > 4 ) {
          _player2->setSetting( 0 );
          _player2->setPoints( 0 );
          _player2->setMode( 1 );
        }
        break;

      // _player2 Games
      case 1:
        _player2->setGames(
            _player2->incrementSetting());
        _gameLeds.updateGames();
        if (_player2->getSetting() > 7) {
          _player2->setSetting( 0 );
          _player2->setGames( 0 );
          _player2->setMode( 2 );
        }
        break;

      // _player2 Sets
      case 2:
        _player2->setSets( _gameState, _player2->incrementSetting());
        _setLeds.updateSets();
        if (_player2->getSetting() > 1 ) {
          _player2->setSetting( 0 );  
          _player2->setSets(_gameState, 0 );
          _player2->setMode( 0 );   
        }
    }
    _gameState->setPreviousTime(_gameState->getNow());  
  } else { /* Serial.println("*** DID NOT ENTER BECAUSE NOT ENOUGH TIME HAS PASSED */ }
}

void Mode2Functions::m2PlayerButtons(int playerButton) {
  switch (playerButton) {
    case 0:
      break;

    case 1:
      _player1->setSetting(_player1->getSetting() + 1);
      _gameState->setNow(_gameState->getNow() + 10000 ); 
      break;

    case 2:
      _player1->setSetting(_player1->getSetting() - 1);
      _gameState->setNow(_gameState->getNow() + 10000 );
      break;

    case 3:
      _player2->setSetting(_player2->getSetting() + 1);
      _gameState->setNow(_gameState->getNow() + 10000 );
      break;

    case 4:
      _player1->setSetting(_player1->getSetting() - 1);
      _gameState->setNow(_gameState->getNow() + 10000 );
      break; }
  _gameState->setPlayerButton( 0 );  }