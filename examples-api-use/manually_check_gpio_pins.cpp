#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <wiringPi.h>

// Function to test GPIO pin availability
bool test_gpio_pin(int pin) {
    // Attempt to set pin mode and toggle state
    try {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, HIGH);
        delay(10);
        digitalWrite(pin, LOW);
        return true;  // Pin can be controlled
    } catch (...) {
        return false;  // Pin is likely in use
    }
}

int main() {
    // List GPIO pins to test
    std::vector<int> pins_to_test = {3, 5, 7, 8, 10, 11, 12, 13, 15, 16, 18, 21, 22, 24, 26, 27};

    // Initialize WiringPi
    if (wiringPiSetupGpio() == -1) {
        std::cerr << "Failed to initialize WiringPi.\n";
        return 1;
    }

    std::cout << "Testing GPIO pin availability:\n";

    for (int pin : pins_to_test) {
        if (test_gpio_pin(pin)) {
            std::cout << "GPIO " << pin << " is available.\n";
        } else {
            std::cout << "GPIO " << pin << " is in use.\n";
        }
    }

    return 0;
}
