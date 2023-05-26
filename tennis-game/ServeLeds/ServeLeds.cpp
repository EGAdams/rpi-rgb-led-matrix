#include "ServeLeds.h"

ServeLeds::ServeLeds(PinInterface* pinInterface, GameState* gameState)
    : _pinInterface(pinInterface), _gameState(gameState) {}
ServeLeds::~ServeLeds() {}

void ServeLeds::updateServeLED() {
  if (_gameState->getServe() /* serve */ == 0) {
    _pinInterface->pinDigitalWrite(P1_SERVE, HIGH);
    _pinInterface->pinDigitalWrite(P2_SERVE, LOW);
  } else {
    _pinInterface->pinDigitalWrite(P1_SERVE, LOW);
    _pinInterface->pinDigitalWrite(P2_SERVE, HIGH);
  }
}

void ServeLeds::serveSwitch() {
  if (_gameState->getServeSwitch() /* serveSwitch */ >= 2) {
    if (_gameState->getServe() /* serve */ == 0) {
      _gameState->setServe(1);  // serve = 1;
    } else {
      _gameState->setServe(0);  // serve = 0;
    }
    _gameState->setServeSwitch(0);  // serveSwitch = 0;
  }
  updateServeLED();
}
