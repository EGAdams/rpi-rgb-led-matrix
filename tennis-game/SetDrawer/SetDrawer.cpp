#include "SetDrawer.h"

SetDrawer::SetDrawer( RGBMatrix* canvas, GameState* gameState ): 
    _canvas( canvas ), _gameState( gameState ) {
    FontLoader smallNumberFontLoader( LITTLE_FONT );
    rgb_matrix::Font smallNumberFont;
    smallNumberFontLoader.LoadFont( smallNumberFont );
    if ( !_little_font.LoadFont( LITTLE_FONT )) { 
        fprintf( stderr, "Couldn't load font '%s'\n", LITTLE_FONT );
        exit( 1 ); }}

SetDrawer::~SetDrawer() { std::cout << "destructing SetDrawer..." << std::endl; }

void SetDrawer::drawSets() {
    Color background_color( 0, 0, 0 );
    int letter_spacing = 0;
    int y = START_ROW; int x = 0;
    rgb_matrix::Font *outline_font = NULL;
    Color thirdRowColor( 0, 255, 0 );
    rgb_matrix::DrawText( _canvas, _little_font, x + SMALL_BEFORE, y + _little_font.baseline(), thirdRowColor,  outline_font ? NULL : &background_color, "0 0 0", letter_spacing );
    y += _little_font.height() - 5;
    Color fourthRowColor( 255, 0, 0 );
    rgb_matrix::DrawText( _canvas, _little_font, x + SMALL_BEFORE, y + _little_font.baseline(), fourthRowColor, outline_font ? NULL : &background_color, "0", letter_spacing );
    rgb_matrix::DrawText( _canvas, _little_font, x + SMALL_BEFORE + SMALL_BETWEEN, y + _little_font.baseline(), fourthRowColor, outline_font ? NULL : &background_color, "0", letter_spacing );
    rgb_matrix::DrawText( _canvas, _little_font, x + SMALL_BEFORE + (( 2 * SMALL_BETWEEN )), y + _little_font.baseline(), fourthRowColor, outline_font ? NULL : &background_color, "0", letter_spacing ); }