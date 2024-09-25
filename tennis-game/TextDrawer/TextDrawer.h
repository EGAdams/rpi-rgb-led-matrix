// TextDrawer.h
#ifndef TEXTDRAWER_H
#define TEXTDRAWER_H

#include "../IDisplay/IDisplay.h"
#include "graphics.h"  // For rgb_matrix::Font and rgb_matrix::Color

class TextDrawer {
public:
    TextDrawer(IDisplay* display, const rgb_matrix::Font& font, const rgb_matrix::Color& color);

    void setFont(const rgb_matrix::Font& font);
    void setColor(const rgb_matrix::Color& color);

    void drawText(const std::string& text, int x, int y);

private:
    IDisplay* _display;
    const rgb_matrix::Font* _font;
    rgb_matrix::Color _color;
};

#endif // TEXTDRAWER_H
