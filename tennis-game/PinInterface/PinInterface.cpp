#include "PinInterface.h"
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

#define MCP23017_ADDRESS 0x20  // Default I2C address when A0, A1, A2 are tied to GND
#define I2C_DEVICE "/dev/i2c-1"
#define GPIO_REGISTER 0x12  // GPIOA register for reading pins


PinInterface::PinInterface( PinState* pinState ) : _pinState( pinState ) {}
PinInterface::~PinInterface() {}

int PinInterface::pinAnalogRead( int pin ) {
    std::string pin_string = std::to_string( pin );
    int pin_value = _pinState->getPinState( pin_string );
    return pin_value; }

int PinInterface::pinDigitalRead( int pin ) {
    int pin_value = _pinState->getPinState( std::to_string( pin ));
    return pin_value; }

void PinInterface::pinAnalogWrite( int pin, int value ) { _pinState->setPinState( std::to_string( pin ), value ); }

void PinInterface::pinDigitalWrite( int pin, int value ) { _pinState->setPinState( std::to_string( pin ), value ); }

std::map<std::string, int> PinInterface::getPinStateMap() { return _pin_map; }

// Function to read bits 0 to 4 from the expander
int PinInterface::_readBits_0_4( int file ) {
    uint8_t reg = GPIO_REGISTER;
    uint8_t value;

    if ( write( file, &reg, 1 ) != 1 ) {
        std::cerr << "Error: Unable to write to I2C device.\n";
        return -1;
    }

    if ( read( file, &value, 1 ) != 1 ) {
        std::cerr << "Error: Unable to read from I2C device.\n";
        return -1;
    }

    uint8_t bits = value & 0x1F;  // Mask bits 0-4

    // std::cout << "Binary value: " << std::bitset<5>( bits ) << "\n";
    // std::cout << "Decimal value: " << static_cast< int >( bits ) << "\n";
    return static_cast< int >( bits );
}

int PinInterface::read_mcp23017_value() {
    // std::cout << "reading MCP23017 bits..." << std::endl;
    int file = open( I2C_DEVICE, O_RDWR );

    if ( file < 0 ) {
        std::cerr << "Error: Unable to open I2C device.\n";
        return -1;
    }

    if ( ioctl( file, I2C_SLAVE, MCP23017_ADDRESS ) < 0 ) {
        std::cerr << "Error: Unable to set I2C address.\n";
        close( file );
        return -1;
    }
    int bits = _readBits_0_4( file );
    close( file );
    return bits;
}
