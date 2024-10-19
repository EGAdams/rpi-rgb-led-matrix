#include "SetDrawer.h"

SetDrawer::SetDrawer( RGBMatrix* canvas, GameState* gameState ) : 
    _canvas( canvas ), _gameState( gameState ), _setHistoryText( gameState ) {
    // std::cout << "constructing SetDrawer..." << std::endl;
    if ( _canvas == NULL ) { std::cout << "canvas is NULL" << std::endl; return; }
    FontLoader smallNumberFontLoader( LITTLE_FONT );
    rgb_matrix::Font smallNumberFont;
    smallNumberFontLoader.LoadFont( smallNumberFont );
    if ( !_little_font.LoadFont( LITTLE_FONT )) { 
        fprintf( stderr, "Couldn't load font '%s'\n", LITTLE_FONT ); exit( 1 ); }}
    
SetDrawer::~SetDrawer() { 
    std::cout << "inside the SetDrawer destructor code... " << std::endl;
    std::cout << "destructing SetDrawer..." << std::endl;
}

void SetDrawer::drawTextOnCanvas( int x, int y, const Color& color, const std::string& text ) {
    Color background_color( 0, 0, 0 );
    int letter_spacing = 0;
    rgb_matrix::Font* outline_font = NULL;
    rgb_matrix::DrawText( _canvas, _little_font, x, y + _little_font.baseline(), color, outline_font ?
                          NULL : &background_color, text.c_str(), letter_spacing ); }

void SetDrawer::drawSets() {
    std::string playerOneSetString = _setHistoryText.getSetHistoryText( PLAYER_ONE_SET_INDEX );
    std::string playerTwoSetString = _setHistoryText.getSetHistoryText( PLAYER_TWO_SET_INDEX );
    if ( _canvas == NULL ) {
        std::cout << playerOneSetString << std::endl;
        std::cout << playerTwoSetString << std::endl;
    } else {
        drawSetsWithSpacing( playerOneSetString, playerTwoSetString );
    }
}

void SetDrawer::blankSets() {
    int y = START_ROW; 
    int x = 0;
    std::string playerOneSetString = "";
    std::string playerTwoSetString = "";
    Color thirdRowColor( 0, 255, 0 );
    if ( _canvas == NULL ) {
        std::cout << playerOneSetString << std::endl;
        std::cout << playerTwoSetString << std::endl;
    } else {
        drawTextOnCanvas( x + SMALL_BEFORE, y, thirdRowColor, playerOneSetString );
        y += _little_font.height() - 5;
        Color fourthRowColor( 255, 0, 0 );
        drawTextOnCanvas( x + SMALL_BEFORE, y, fourthRowColor, playerTwoSetString ); }}

void SetDrawer::drawBlinkSets( int playerToBlink ) {
    int y = START_ROW; 
    int x = 0;
    int set = _gameState->getCurrentSet(); // init coords and set
    std::string playerOneSetString = ""; std::string playerTwoSetString = ""; // set inside if statement
    if ( _gameState->getCurrentAction() == BOTH_PLAYER_BLINK ) {
        playerOneSetString = cloaker( _setHistoryText.getSetHistoryText( PLAYER_ONE_SET_INDEX ), set );
        playerTwoSetString = cloaker( _setHistoryText.getSetHistoryText( PLAYER_TWO_SET_INDEX ), set );
    } else if ( playerToBlink == PLAYER_1_INITIALIZED ) { // Blink player 1
        playerOneSetString = cloaker( _setHistoryText.getSetHistoryText( PLAYER_ONE_SET_INDEX ), set );
        playerTwoSetString = _setHistoryText.getSetHistoryText( PLAYER_TWO_SET_INDEX );
    } else {                                       // Blink player 2
        playerOneSetString = _setHistoryText.getSetHistoryText( PLAYER_ONE_SET_INDEX );
        playerTwoSetString = cloaker( _setHistoryText.getSetHistoryText( PLAYER_TWO_SET_INDEX ), set ); }
    
    if ( _canvas == NULL ) {
        std::cout << playerOneSetString << std::endl;
        std::cout << playerTwoSetString << std::endl;
    } else {
        Color thirdRowColor( 0, 255, 0 );
        drawTextOnCanvas( x + SMALL_BEFORE, y, thirdRowColor, playerOneSetString );
        y += _little_font.height() - 5;
        Color fourthRowColor( 255, 0, 0 );
        drawTextOnCanvas( x + SMALL_BEFORE, y, fourthRowColor, playerTwoSetString ); }}

std::string SetDrawer::cloaker( std::string stringToCloak, int sectionToCloak ) {
    if ( sectionToCloak < 1 || sectionToCloak > 3 ) { return "Invalid section number";}
    int pos = 2 * ( sectionToCloak - 1 ); // The pos of the digit in the string is ( 2 * section number - 2 ) ( the 1st digit is at position 0 )
    stringToCloak[ pos ] = ' ';           // Replace the character at the calculated position with a space
    return stringToCloak;
}

#include <sstream>  // Include for string stream operations
#include <vector>
#include <string>

// Assume these are defined elsewhere in your code
// struct Color { int r, g, b; Color(int red, int green, int blue) : r(red), g(green), b(blue) {} };
// void drawTextOnCanvas(int x, int y, Color color, const std::string& text);
// int START_ROW;
// int SMALL_BEFORE;
// class Font { public: int width(const std::string& text); int height(); } _little_font;

std::vector<std::string> SetDrawer::splitString(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, delimiter)) {
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }

    return tokens;
}

void SetDrawer::drawPlayerSets(const std::vector<std::string>& sets, Color color, int& x, int y) {
    const int OFFSET_FOR_ONE = 2;       // Offset in pixels for set "1" (adjust as needed)
    const int SET_SPACING = 10;         // Spacing between sets (adjust as needed)

    for (const std::string& set : sets) {
        // Determine offset based on the set value
        int offset = 0;
        if (set == "1") {
            offset = OFFSET_FOR_ONE;
        }

        // Draw the set at the current x position plus any offset
        drawTextOnCanvas(x + SMALL_BEFORE + offset, y, color, set);

        // Advance the x position by the width of the set plus spacing
        // Assuming _little_font.width(set) returns the width of the set text
        x += /* _little_font.width(set) */ SMALL_BEFORE + offset + SET_SPACING;
    }
}

void SetDrawer::drawSetsWithSpacing(std::string playerOneSetString, std::string playerTwoSetString) {
    // Define initial y positions and colors
    int yPlayerOne = START_ROW; 
    int yPlayerTwo = START_ROW + _little_font.height() - 5; // Move to the next row

    Color playerOneColor(0, 255, 0);    // Green for Player One
    Color playerTwoColor(255, 0, 0);    // Red for Player Two

    // Split the set strings into individual sets
    std::vector<std::string> playerOneSets = splitString(playerOneSetString);
    std::vector<std::string> playerTwoSets = splitString(playerTwoSetString);

    // Initialize x positions for both players
    int xPlayerOne = 0;
    int xPlayerTwo = 0;

    // Draw Player One's sets
    drawPlayerSets(playerOneSets, playerOneColor, xPlayerOne, yPlayerOne);

    // Draw Player Two's sets
    drawPlayerSets(playerTwoSets, playerTwoColor, xPlayerTwo, yPlayerTwo);
}
