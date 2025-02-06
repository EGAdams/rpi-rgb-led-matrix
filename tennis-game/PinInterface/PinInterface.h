#ifndef PININTERFACE_H
#define PININTERFACE_H

#include "../TennisConstants/TennisConstants.h"
#include "../PinState/PinState.h"
#include "../Arduino/Arduino.h"
#include "../TennisConstants/TennisConstants.h"
#include <map>
#include <string>

// for the expander
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

// to set the bits
#include <bitset>

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
