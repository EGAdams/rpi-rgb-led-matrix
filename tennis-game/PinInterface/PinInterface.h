#ifndef PININTERFACE_H
#define PININTERFACE_H

#include "../TennisConstants/TennisConstants.h"
#include "../PinState/PinState.h"
#include "../Arduino/Arduino.h"
#include <map>

class PinInterface {
  public:
    PinInterface(PinState* pinState);
    ~PinInterface();
    void pinAnalogWrite(int pin, int value);
    void pinDigitalWrite(int pin, int value);
    int pinAnalogRead(int pin);
    int pinDigitalRead(int pin);
    std::map<std::string, int> getPinStateMap();
    int read_mcp23017_value();

  private:
    std::map<std::string, int> _pin_map;
    PinState* _pinState;
    int _readBits_0_4( int file );                    
};

#endif
