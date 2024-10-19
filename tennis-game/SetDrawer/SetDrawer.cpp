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

void SetDrawer::drawPlayerSets(const std::vector<std::string>& sets, Color color, int y) {
    const int OFFSET_FOR_ONE = 2;       // Offset in pixels for set "1" (adjust as needed)
    const int SET_SPACING = 10;         // Spacing between sets (adjust as needed)
    for (size_t i = 0; i < sets.size(); ++i) {
        const std::string& set = sets[i];
        
        // Calculate the starting x position for this set
        int set_x = SMALL_BEFORE + static_cast<int>(i) * (FIXED_SET_WIDTH + SET_SPACING);
        
        // Determine if an offset is needed
        int offset = (set == "1") ? OFFSET_FOR_ONE : 0;

        // Calculate the final x position within the fixed set width
        int final_x = set_x + offset;

        // Draw the set text
        drawTextOnCanvas(final_x, y, color, set);
    }
}

void SetDrawer::drawSetsWithSpacing(const std::string& playerOneSetString, const std::string& playerTwoSetString) {
    // Split the set strings into individual sets
    std::vector<std::string> playerOneSets = splitString(playerOneSetString);
    std::vector<std::string> playerTwoSets = splitString(playerTwoSetString);

    // Define colors for players
    Color playerOneColor(0, 255, 0);    // Green for Player One
    Color playerTwoColor(255, 0, 0);    // Red for Player Two

    // Define y positions for each player
    int yPlayerOne = START_ROW;
    int yPlayerTwo = START_ROW + _little_font.height() - 5; // Move to the next row

    // Draw Player One's sets
    drawPlayerSets(playerOneSets, playerOneColor, yPlayerOne);

    // Draw Player Two's sets
    drawPlayerSets(playerTwoSets, playerTwoColor, yPlayerTwo);
}