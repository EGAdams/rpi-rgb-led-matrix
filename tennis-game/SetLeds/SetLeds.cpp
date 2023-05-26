#include "SetLeds.h"
#include "../Arduino/Arduino.h"
#include "../TennisConstants/TennisConstants.h"

SetLeds::SetLeds(Player* player1, Player* player2, PinInterface* pinInterface)
    : _player1(player1), _player2(player2), _pinInterface(pinInterface) {}

SetLeds::~SetLeds() {}

void SetLeds::updateSets() {
  switch (_player1->getSets() ) {
    case 0:
      _pinInterface->pinDigitalWrite(P1_SETS_LED1, LOW);
      _pinInterface->pinDigitalWrite(P1_SETS_LED2, LOW);
      break;
    case 1:
      _pinInterface->pinDigitalWrite(P1_SETS_LED1, HIGH);
      _pinInterface->pinDigitalWrite(P1_SETS_LED2, LOW);
      break;
    case 2:
      _pinInterface->pinDigitalWrite(P1_SETS_LED1, HIGH);
      _pinInterface->pinDigitalWrite(P1_SETS_LED2, HIGH);
      break;
    case 3:
      _pinInterface->pinDigitalWrite(P1_SETS_LED1, HIGH);
      _pinInterface->pinDigitalWrite(P1_SETS_LED2, HIGH);
      break;
  }

  switch (_player2->getSets()) {
    case 0:
      _pinInterface->pinDigitalWrite(P2_SETS_LED1, LOW);
      _pinInterface->pinDigitalWrite(P2_SETS_LED2, LOW);
      break;
    case 1:
      _pinInterface->pinDigitalWrite(P2_SETS_LED1, HIGH);
      _pinInterface->pinDigitalWrite(P2_SETS_LED2, LOW);
      break;
    case 2:
      _pinInterface->pinDigitalWrite(P2_SETS_LED1, HIGH);
      _pinInterface->pinDigitalWrite(P2_SETS_LED2, HIGH);
      break;
    case 3:
      _pinInterface->pinDigitalWrite(P2_SETS_LED1, HIGH);
      _pinInterface->pinDigitalWrite(P2_SETS_LED2, HIGH);
      break;
  }
}
