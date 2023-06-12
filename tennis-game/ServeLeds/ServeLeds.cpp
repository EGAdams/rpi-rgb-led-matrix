#include "ServeLeds.h"

ServeLeds::ServeLeds(PinInterface* pinInterface, GameState* gameState)
    : _pinInterface(pinInterface), _gameState(gameState) {}
ServeLeds::~ServeLeds() {}

void ServeLeds::updateServeLED() {
  if (_gameState->getServe() /* serve */ == 0) {
    std::cout << "updateServeLED() setting P1_SERVE to HIGH" << std::endl;
    _pinInterface->pinDigitalWrite(P1_SERVE, HIGH);
    _pinInterface->pinDigitalWrite(P2_SERVE, LOW);
  } else {
    std::cout << "updateServeLED() setting P2_SERVE to HIGH" << std::endl;
    _pinInterface->pinDigitalWrite(P1_SERVE, LOW);
    _pinInterface->pinDigitalWrite(P2_SERVE, HIGH);
  }
}

void ServeLeds::serveSwitch() {
  if (_gameState->getServeSwitch() /* serveSwitch */ >= 2) {
    if (_gameState->getServe() /* serve */ == 0) {
      std::cout << "serveSwitch() setting serve to 1" << std::endl;
      _gameState->setServe( 1 );  // serve = 1;
    } else {
      std::cout << "serveSwitch() setting serve to 0" << std::endl;
      _gameState->setServe( 0 );  // serve = 0;
    }
    std::cout << "serveSwitch() setting serveSwitch to 0" << std::endl;
    _gameState->setServeSwitch ( 0 );  // serveSwitch = 0;
  }
  std::cout << "updateServeLED() called from serveSwitch()" << std::endl;
  updateServeLED();
}
