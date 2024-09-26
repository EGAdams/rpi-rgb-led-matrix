#include "ConsoleDisplay.h"

ConsoleDisplay::ConsoleDisplay() {
    // Constructor implementation
}
ConsoleDisplay::~ConsoleDisplay() {
    // Destructor implementation
}

void ConsoleDisplay::drawText(const std::string& text, int x, int y, const rgb_matrix::Color& color, const rgb_matrix::Font& font) {
    std::cout << "Drawing text: " << text << " at (" << x << ", " << y << ")" << std::endl;
}

void ConsoleDisplay::clearScreen() {
    std::cout << "Clearing screen" << std::endl;
    
    // ANSI escape code to clear the screen
    std::cout << "\033[2J\033[H";
}
