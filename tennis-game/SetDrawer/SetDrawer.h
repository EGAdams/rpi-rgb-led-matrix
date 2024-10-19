#ifndef SET_MANAGER_H
#define SET_MANAGER_H

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
    void drawSetsWithSpacing( std::string playerOneSetString, std::string playerTwoSetString );

 private: 
    rgb_matrix::Font    _little_font;
    RGBMatrix*          _canvas;
    GameState*          _gameState; 
    SetHistoryText      _setHistoryText;

    // Helper function to split a string by spaces into a vector of strings
    std::vector<std::string> splitString( const std::string& str, char delimiter = ' ' );

    // Helper function to draw sets for a player
    void drawPlayerSets( const std::vector<std::string>& sets, Color color, int& x, int y );
};
#endif