#include <iostream>
#include <unistd.h>
#include "../lib/gpio.h"  // Make sure this path is correct based on your setup.

using namespace rgb_matrix;

int main() {
    #define GPIO_BIT(b) ((uint64_t)1<<(b))
    GPIO io;
    if (!io.Init(1)) {  // Initialize GPIO with slowdown parameter set to 1.
        std::cerr << "Failed to initialize GPIO" << std::endl;
        return 1;
    }

    // Define the GPIO pins to be used as inputs.
    gpio_bits_t input_pins = GPIO_BIT(14) | GPIO_BIT(16) | GPIO_BIT(21) | GPIO_BIT(26);

    // Request the GPIO pins as inputs.
    gpio_bits_t available_inputs = io.RequestInputs(input_pins);
    if (available_inputs != input_pins) {
        std::cerr << "Not all requested GPIO inputs could be reserved" << std::endl;
        return 1;
    }

    std::cout << "Reading GPIO inputs..." << std::endl;

    while (true) {
        gpio_bits_t values = io.Read();
        gpio_bits_t input_values = values & input_pins;

        // Extract individual bits from the input values.
        int bit0 = (input_values & GPIO_BIT(14)) ? 1 : 0;
        int bit1 = (input_values & GPIO_BIT(16)) ? 1 : 0;
        int bit2 = (input_values & GPIO_BIT(21)) ? 1 : 0;
        int bit3 = (input_values & GPIO_BIT(26)) ? 1 : 0;

        // Assemble the bits into a 4-bit number.
        int value = (bit3 << 3) | (bit2 << 2) | (bit1 << 1) | bit0;

        std::cout << "" << std::endl;
        std::cout << "bit 0: " << bit0 << std::endl;
        std::cout << "bit 1: " << bit1 << std::endl;
        std::cout << "bit 2: " << bit2 << std::endl;
        std::cout << "bit 3: " << bit3 << std::endl;
        std::cout << "Input value: " << value << std::endl;
        std::cout << "" << std::endl;

        usleep(50000); // Sleep for 50 milliseconds.
    }

    return 0;
}
