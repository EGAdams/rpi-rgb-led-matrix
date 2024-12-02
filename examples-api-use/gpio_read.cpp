#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <unistd.h>

#define SYSFS_GPIO_DIR "/sys/class/gpio/"

// Function to export a GPIO pin
void export_gpio(int gpio) {
    std::ofstream export_file(SYSFS_GPIO_DIR "export");
    if (export_file.is_open()) {
        export_file << gpio;
        export_file.close();
    } else {
        std::cerr << "Error: Unable to export GPIO " << gpio << std::endl;
        exit(EXIT_FAILURE);
    }
}

// Function to set the direction of a GPIO pin
void set_gpio_direction(int gpio, const std::string& direction) {
    std::ofstream direction_file(SYSFS_GPIO_DIR "gpio" + std::to_string(gpio) + "/direction");
    if (direction_file.is_open()) {
        direction_file << direction;
        direction_file.close();
    } else {
        std::cerr << "Error: Unable to set direction for GPIO " << gpio << std::endl;
        exit(EXIT_FAILURE);
    }
}

// Function to read the value of a GPIO pin
int read_gpio(int gpio) {
    std::ifstream value_file(SYSFS_GPIO_DIR "gpio" + std::to_string(gpio) + "/value");
    int value = -1;
    if (value_file.is_open()) {
        value_file >> value;
        value_file.close();
    } else {
        std::cerr << "Error: Unable to read value for GPIO " << gpio << std::endl;
    }
    return value;
}

int main() {
    // GPIO pins to test
    std::vector<int> gpio_pins = { 14, 16, 21, 26 };

    // Export and configure GPIO pins as inputs
    for (int gpio : gpio_pins) {
        export_gpio(gpio);
        usleep(100000); // Wait for sysfs setup
        set_gpio_direction(gpio, "in");
    }

    std::cout << "Reading GPIO pin values. Apply voltage and check results (3.3V is standard)." << std::endl;

    // Read and display the values of the GPIO pins
    for (int gpio : gpio_pins) {
        int value = read_gpio(gpio);
        if (value != -1) {
            std::cout << "GPIO " << gpio << " value: " << value << std::endl;
        }
    }

    return 0;
}
