In the following class, the method to draw the sets is making the "1" flicker when other set numbers are changed around it.  I has something to do with how the "1" is drawn with the offset.  Maybye we can fix it with buffering?  Please modify the following C++ code to accomplish this:
```cpp
#ifndef SET_MANAGER_H
#define SET_MANAGER_H

#include <sstream>  // Include for string stream operations
#include <vector>
#include <iostream>
#include <string>
#include "../GameState/GameState.h"
#include "../Player/Player.h"
#include "../Drawer/Drawer.h"
#include "../FontLoader/FontLoader.h"
#include "../SetHistoryText/SetHistoryText.h"
#include "../TennisConstants/TennisConstants.h"

#define SMALL_BEFORE  7
#define SMALL_BETWEEN 17
#define START_ROW     86
#define LITTLE_FONT   "fonts/the_sets_font.bdf"


class SetDrawer {
 public:
    SetDrawer( RGBMatrix* canvas, GameState* gameState );
    SetDrawer();
    ~SetDrawer();
    std::string  cloaker( std::string stringToCloak, int sectionToCloak );
    void drawSets();
    void drawBlinkSets( int player);
    void drawTextOnCanvas( int x, int y, const Color& color, const std::string& text );
    void blankSets();

 private: 
   const int FIXED_SET_WIDTH = 20;  // Adjust as needed based on your font size
   const int SET_SPACING = 10;      // Space between sets
   const int OFFSET_FOR_ONE = 2;    // Additional offset for the digit "1"

    rgb_matrix::Font    _little_font;
    RGBMatrix*          _canvas;
    GameState*          _gameState; 
    SetHistoryText      _setHistoryText;

    // Helper function to split a string by spaces into a vector of strings
    std::vector<std::string> splitString( const std::string& str, char delimiter = ' ' );

    int getStringWidth(const std::string& text);

    void drawPlayerSets(const std::vector<std::string>& sets, Color color, int& x, int y);

    void drawSetsWithSpacing(std::string playerOneSetString, std::string playerTwoSetString);
};
#endif

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

// Helper function to calculate the width of a string by summing character widths
int SetDrawer::getStringWidth(const std::string& text) {
    int totalWidth = 0;
    for (char c : text) {
        // Convert char to Unicode codepoint (assuming ASCII)
        uint32_t codepoint = static_cast<uint32_t>(c);
        int charWidth = _little_font.CharacterWidth(codepoint);
        if (charWidth == -1) {
            // Handle missing character by skipping or using a default width
            // Here, we'll skip adding width for missing characters
            continue;
        }
        totalWidth += charWidth;
    }
    return totalWidth;
}

// Helper function to draw sets for a single player
void SetDrawer::drawPlayerSets(const std::vector<std::string>& sets, Color color, int& x, int y) {
    for (const std::string& set : sets) {
        // Determine offset based on the set value
        int offset = (set == "1") ? OFFSET_FOR_ONE : 0;

        // Draw the set at the current x position plus any offset
        drawTextOnCanvas(x + SMALL_BEFORE + offset, y, color, set);

        // Calculate the width of the set string
        int setWidth = getStringWidth(set);

        // Advance the x position by the width of the set plus spacing
        x += setWidth + SET_SPACING;
    }
}

// Main function to draw sets with spacing for both players
void SetDrawer::drawSetsWithSpacing(std::string playerOneSetString, std::string playerTwoSetString) {
    // Split the set strings into individual sets
    std::vector<std::string> playerOneSets = splitString(playerOneSetString);
    std::vector<std::string> playerTwoSets = splitString(playerTwoSetString);

    // Define colors for players
    Color playerOneColor(0, 255, 0);    // Green for Player One
    Color playerTwoColor(255, 0, 0);    // Red for Player Two

    // Define y positions for each player
    int yPlayerOne = START_ROW;
    int yPlayerTwo = START_ROW + _little_font.height() - 5; // Move to the next row

    // Initialize x positions for both players
    int xPlayerOne = 0;
    int xPlayerTwo = 0;

    // Draw Player One's sets
    drawPlayerSets(playerOneSets, playerOneColor, xPlayerOne, yPlayerOne);

    // Draw Player Two's sets
    drawPlayerSets(playerTwoSets, playerTwoColor, xPlayerTwo, yPlayerTwo);
}
```