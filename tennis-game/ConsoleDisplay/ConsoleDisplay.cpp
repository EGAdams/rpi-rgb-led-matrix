// ConsoleDisplay.cpp
#include "ConsoleDisplay.h"

void ConsoleDisplay::drawText(const std::string& text, int x, int y, const rgb_matrix::Color& color, const rgb_matrix::Font& font) {
    // For console output, we can simulate color using ANSI escape codes if desired.
    std::cout << text << std::endl;
}

void ConsoleDisplay::clearScreen() {
    // ANSI escape code to clear the screen
    std::cout << "\033[2J\033[H";
}
