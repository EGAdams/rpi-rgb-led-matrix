#include "MatrixDisplay.h"
#include <iostream>

MatrixDisplay::MatrixDisplay( rgb_matrix::RGBMatrix* canvas ) : _canvas( canvas ) {
    _currentFont  = FontManager::getFont(   "defaultFont" );  // Assuming FontManager provides default font
    _currentColor = ColorManager::getColor( "WHITE"       );  // Default color is white
}
MatrixDisplay::~MatrixDisplay() { if ( _canvas ) { delete _canvas; _canvas = nullptr; }}

void MatrixDisplay::setFont(  const std::string& fontName ) {  _currentFont  = FontManager::getFont(   fontName );  }
void MatrixDisplay::setColor( const std::string& colorName ) { _currentColor = ColorManager::getColor( colorName ); }

void MatrixDisplay::drawText( const std::string& text, int x, int y ) {
    if ( !_canvas ) { std::cerr << "Canvas is not initialized. Cannot draw text." << std::endl; return; }
    rgb_matrix::DrawText( _canvas, _currentFont, x, y + _currentFont.baseline(), _currentColor, nullptr, text.c_str(), 0 );
}

void MatrixDisplay::clearScreen() {
    if ( _canvas ) { _canvas->Clear();  // Clear the RGB matrix canvas
    } else { std::cerr << "Canvas is not initialized. Cannot clear screen." << std::endl; }
}

