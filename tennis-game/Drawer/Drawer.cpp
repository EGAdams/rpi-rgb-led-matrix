#include "Drawer.h"

Drawer::Drawer( RGBMatrix* canvas, const rgb_matrix::Font* font, Size size, const Color& color, const Color& bg_color )
    : _canvas( canvas ), _font( font ), _size( size ), _color( color ), _bg_color( bg_color ) {}

void Drawer::drawNumber( const std::string& number, int x, int y ) {
    // return value of DrawText() goes to the bit bucket
    rgb_matrix::DrawText( _canvas, *_font, x, y, _color, &_bg_color, number.c_str(), 0 ); }

void Drawer::drawText( const std::string& text, int x, int y ) {
    // return value of DrawText() goes to the bit bucket
    rgb_matrix::DrawText( _canvas, *_font, x, y, _color, &_bg_color, text.c_str(), 0 ); }
