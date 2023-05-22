
#include "NumberDrawer.h"

class NumberDrawer {
public:
    enum Size { BIG, SMALL };

    NumberDrawer(RGBMatrix* canvas, const rgb_matrix::Font& font, Size size, const Color& color, const Color& bg_color)
        : canvas_(canvas), font_(font), size_(size), color_(color), bg_color_(bg_color) {}

    void DrawNumber(const std::string& number, int x, int y) {
        rgb_matrix::DrawText(canvas_, font_, x, y, color_, &bg_color_, number.c_str(), 0);
    }

private:
    RGBMatrix* canvas_;
    rgb_matrix::Font font_;
    Size size_;
    Color color_;
    Color bg_color_;
};
