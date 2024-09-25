#ifndef IDISPLAY_H
#define IDISPLAY_H
#include <string>
#include "../../include/graphics.h"

using namespace rgb_matrix;

class IDisplay {
public:
    virtual void drawText(const std::string& text, int x, int y, const Color& color, const Font& font) = 0;
    virtual void clearScreen() = 0;
    virtual ~IDisplay() = default;
};
#endif // IDISPLAY_H