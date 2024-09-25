// TextDrawer.cpp
#include "TextDrawer.h"

TextDrawer::TextDrawer(IDisplay* display, const rgb_matrix::Font& font, const rgb_matrix::Color& color)
    : _display(display), _font(&font), _color(color) {}

void TextDrawer::setFont(const rgb_matrix::Font& font) {
    _font = &font;
}

void TextDrawer::setColor(const rgb_matrix::Color& color) {
    _color = color;
}

void TextDrawer::drawText(const std::string& text, int x, int y) {
    _display->drawText(text, x, y, _color, *_font);
}
