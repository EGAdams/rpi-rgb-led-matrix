#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// Function to check if a GPIO pin is in use
bool is_gpio_in_use(int gpio) {
    std::string gpio_path = "/sys/class/gpio/gpio" + std::to_string(gpio);
    std::ifstream gpio_dir(gpio_path);
    return gpio_dir.good();
}

int main() {
    // List of GPIO pins to check
    std::vector<int> pins_to_check = {4, 17, 27, 22};

    std::cout << "Checking GPIO pin usage:\n";

    for (int pin : pins_to_check) {
        if (is_gpio_in_use(pin)) {
            std::cout << "GPIO " << pin << " is in use.\n";
        } else {
            std::cout << "GPIO " << pin << " is available.\n";
        }
    }

    return 0;
}
