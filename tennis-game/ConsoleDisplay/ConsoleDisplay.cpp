#include "ConsoleDisplay.h"

void ConsoleDisplay::drawText(const std::string& text, int x, int y, const rgb_matrix::Color& color, const rgb_matrix::Font& font) {
    // Implement drawing text to console
    std::cout << "Drawing text: " << text << " at (" << x << ", " << y << ")" << std::endl;
}

void ConsoleDisplay::clearScreen() {
    // Implement clearing the console screen
    std::cout << "Clearing screen" << std::endl;
    
    // ANSI escape code to clear the screen
    std::cout << "\033[2J\033[H";
}
