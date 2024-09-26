#ifndef CONSOLEDISPLAY_H
#define CONSOLEDISPLAY_H
#include "../IDisplay/IDisplay.h"
#include <iostream>

class ConsoleDisplay : public IDisplay {
public:
    ConsoleDisplay() = default;  // Add this line
    void drawText(const std::string& text, int x, int y, const rgb_matrix::Color& color, const rgb_matrix::Font& font) override;
    void clearScreen() override;
    ~ConsoleDisplay() override = default;
};
#endif // CONSOLEDISPLAY_H