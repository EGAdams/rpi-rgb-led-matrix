#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>

// List of GPIO pins to exclude (VCC, GND, and CLK)
const std::set<int> EXCLUDED_PINS = {
    // Physical pins to exclude
    1,   // 3.3V Power
    2,   // 5V Power
    4,   // 5V Power
    6,   // Ground
    9,   // Ground
    14,  // Ground
    17,  // 3.3V Power
    20,  // Ground
    25,  // Ground
    30,  // Ground
    34,  // Ground
    39,  // Ground
    19,  // CLK (GPIO 10 SPI_MOSI)
    23,  // CLK (GPIO 11 SPI_CLK)
};

// Function to check if a GPIO pin is in use
bool is_gpio_in_use(int gpio) {
    std::string gpio_path = "/sys/class/gpio/gpio" + std::to_string(gpio);
    std::ifstream gpio_dir(gpio_path);
    return gpio_dir.good();
}

int main() {
    std::cout << "Checking GPIO pin usage:\n";

    // Check all usable GPIO pins (2 to 27 for Pi Zero 2)
    for (int gpio = 2; gpio <= 27; ++gpio) {
        // Skip excluded pins
        if (EXCLUDED_PINS.find(gpio) != EXCLUDED_PINS.end()) {
            continue;
        }

        // Check if the pin is in use
        if (is_gpio_in_use(gpio)) {
            std::cout << "GPIO " << gpio << " is in use.\n";
        } else {
            std::cout << "GPIO " << gpio << " is available.\n";
        }
    }

    return 0;
}
