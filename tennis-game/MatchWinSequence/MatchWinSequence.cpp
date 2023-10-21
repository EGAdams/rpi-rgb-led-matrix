#include "MatchWinSequence.h"
#include <iostream>

MatchWinSequence::MatchWinSequence()  {}
MatchWinSequence::~MatchWinSequence() {}

void MatchWinSequence::_showText( ScoreBoard* scoreboard, std::string message, int first_offset, int baseline ) {
    FontLoader bigNumberFontLoader( BIG_NUMBER_FONT );
    rgb_matrix::Font big_number_font;
    bigNumberFontLoader.LoadFont( big_number_font );
    if (!big_number_font.LoadFont( BIG_NUMBER_FONT )) {
        fprintf( stderr, "Couldn't load font '%s'\n", BIG_NUMBER_FONT ); exit( 1 );}
    Color color( 255, 255, 0 );
    Color bg_color( 0, 0, 0 );
    Drawer drawer ( scoreboard->getCanvas(), &big_number_font, Drawer::SMALL, color, bg_color );
    drawer.drawText( message, first_offset, baseline + big_number_font.height()); // draw the text
}

void MatchWinSequence::run( Player* player, GameState* gameState, GameLeds* gameLeds, SetLeds* setLeds ) {
    std::cout << "//////////////////////// MatchWinSequence::run() ////////////////////////" << std::endl;
    GameTimer::gameDelay( MATCH_WIN_FLASH_DELAY );
    if ( gameLeds->getScoreBoard()->hasCanvas()) {
        std::cout << "scoreboard has canvas.  updating..." << std::endl;
        gameLeds->getScoreBoard()->clearCanvas();
        _showText( gameLeds->getScoreBoard(), "MATCH", 2, 20 );
        GameTimer::gameDelay( 5 );
    } else {
        for ( int blink_sequence_count = 0; blink_sequence_count < LOOP_MATCH_LAMP_WIN; blink_sequence_count++ ) {
            for ( int current_lamp = 0; current_lamp < NUMBER_OF_GAME_LAMPS; current_lamp++ ) {
                player->setGames( 6 );
                gameLeds->updateGames();
                GameTimer::gameDelay( MATCH_WIN_FLASH_DELAY );
                player->setGames( current_lamp );
                gameLeds->updateGames();
                GameTimer::gameDelay( MATCH_WIN_FLASH_DELAY ); }
            GameTimer::gameDelay( MATCH_WIN_FLASH_DELAY ); }} // end LOOP_MATCH_LAMP_WIN loop  // end run().
}