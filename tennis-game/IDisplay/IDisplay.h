#ifndef IDISPLAY_H
#define IDISPLAY_H
#include <string>
#include "../../include/graphics.h"  // Adjust this path as necessary

class IDisplay {
public:
    virtual void drawText(const std::string& text, int x, int y, const rgb_matrix::Color& color, const rgb_matrix::Font& font) = 0;
    virtual void clearScreen() = 0;
    virtual ~IDisplay() = default;
};

#endif // IDISPLAY_H