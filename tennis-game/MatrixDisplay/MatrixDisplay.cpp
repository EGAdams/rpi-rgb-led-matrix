#include "MatrixDisplay.h"
#include <iostream>

MatrixDisplay::MatrixDisplay( rgb_matrix::RGBMatrix* canvas, Drawer* drawer ) : _canvas( canvas ), _drawer( drawer ) {
    _currentFont  = FontManager::getFont(   LITTLE_NUMBER_FONT );  // Assuming FontManager provides default font
    _currentColor = ColorManager::getColor( "WHITE" );  // Default color is white
    _bg_color     = ColorManager::getColor( "BLACK" );  // Default background color is black
}
MatrixDisplay::~MatrixDisplay() { if ( _canvas ) { delete _canvas; _canvas = nullptr; }}

void MatrixDisplay::setFont(const std::string& fontName) {  
    _currentFont = FontManager::getFont(fontName); 
    _drawer->setFont( &_currentFont );
}

void MatrixDisplay::setColor( const std::string& colorName ) { _currentColor = ColorManager::getColor( colorName ); }

// void Drawer::drawText( const std::string& text, int x, int y ) {
//     rgb_matrix::DrawText( _canvas, *_font, x, y, _color, &_bg_color, text.c_str(), 0 );
// }

void MatrixDisplay::drawText( const std::string& text, int x, int y ) {
    if ( !_canvas ) { std::cerr << "Canvas is not initialized. Cannot draw text." << std::endl; return; }
    // rgb_matrix::DrawText( _canvas, _currentFont, x, y + _currentFont.baseline(), _currentColor, &_bg_color, text.c_str(), 0 );
    print( "inside MatrixDisplay::drawText.  about to call _drawer->drawText..." );
    _drawer->drawText( text, x, y );
}

void MatrixDisplay::clearScreen() {
    if ( _canvas ) { _canvas->Clear();  // Clear the RGB matrix canvas
    } else { std::cerr << "Canvas is not initialized. Cannot clear screen." << std::endl; }
}

