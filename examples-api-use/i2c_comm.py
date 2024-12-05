#!/usr/bin/env python3
import smbus
import time

# Create an SMBus instance
bus = smbus.SMBus( 1 )  # Use I2C bus 1 on Raspberry Pi

# MCP23017 default I2C address (0x20 when A0, A1, A2 are tied to GND)
DEVICE_ADDRESS = 0x20

# MCP23017 Register Addresses
IODIRA = 0x00    # I/O direction register for Port A
GPPUA  = 0x0C    # Pull-up resistor configuration for Port A
GPIOA  = 0x12    # GPIO register for Port A

def setup():
    # Configure Port A pins as inputs
    # Set PA0-PA3 as inputs (1 = input, 0 = output)
    bus.write_byte_data(DEVICE_ADDRESS, IODIRA, 0x0F)
    
    # Enable internal pull-up resistors on PA0-PA3
    bus.write_byte_data(DEVICE_ADDRESS, GPPUA, 0x0F)

def read_inputs():
    # Read the state of GPIOA
    input_state = bus.read_byte_data(DEVICE_ADDRESS, GPIOA)
    
    # Mask out the lower 4 bits (PA0-PA3)
    input_value = input_state & 0x0F
    
    # Invert the bits if using pull-up resistors (active low)
    # This converts the reading to active high logic
    input_value = ~input_value & 0x0F
    
    # Convert the binary input to decimal
    decimal_value = input_value
    
    return input_value, decimal_value

def main():
    setup()
    try:
        while True:
            binary_input, decimal_output = read_inputs()
            print(f"Binary Input: {binary_input:04b}, Decimal Output: {decimal_output}")
            time.sleep(0.5)
    except KeyboardInterrupt:
        print("\nProgram terminated by user")

if __name__ == '__main__':
    main()
