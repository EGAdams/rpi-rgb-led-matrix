#include "MatrixTextDrawer.h"

MatrixTextDrawer::MatrixTextDrawer( IDisplay* display, const std::string& font_name, const std::string& color_name )
    : _display( display ), _font_name( font_name ), _color_name( color_name ) {}

void MatrixTextDrawer::setFont( const std::string& font_name ) {
    _font_name = font_name;
}

void MatrixTextDrawer::setColor( const std::string& color_name ) {
    _color_name = color_name;
}

void MatrixTextDrawer::drawText( const std::string& text, int x, int y ) {
    _display->drawText( text, x, y );
}
