#ifndef NUMBER_DRAWER_H
#define NUMBER_DRAWER_H

#include "led-matrix.h"
#include "graphics.h"

using namespace rgb_matrix;

class NumberDrawer {
public:
    enum Size { BIG, SMALL };

    NumberDrawer(RGBMatrix* canvas, const rgb_matrix::Font* font, Size size, const Color& color, const Color& bg_color);

    void DrawNumber(const std::string& number, int x, int y);

private:
    RGBMatrix* canvas_;
    const rgb_matrix::Font* font_; // Changed this from a reference to a pointer
    Size size_;
    Color color_;
    Color bg_color_;
};

#endif // NUMBER_DRAWER_H
