#include "SetWin.h"

SetWin::SetWin( Undo* undo, GameState* gameState, SetLeds* setLeds ): 
    _undo( undo ), _gameState( gameState ), _setLeds( setLeds ) {}
SetWin::~SetWin() {}

void SetWin::execute( Player* player, ScoreBoard* scoreBoard ) {
    std::cout << "//////////////////////// SetWin::execute() ////////////////////////" << std::endl;
     for ( int blink_sequence_count = 0; blink_sequence_count < LOOP_GAME_LAMP_WIN; blink_sequence_count++ ) {
            _gameState->setCurrentAction( BOTH_PLAYER_BLINK );
            scoreBoard->update();
            GameTimer::gameDelay( SET_WIN_FLASH_DELAY );  /*** wait ***/
            _gameState->setCurrentAction( "normal operation" );
            scoreBoard->update(); 
            GameTimer::gameDelay( SET_WIN_FLASH_DELAY );  /*** wait ***/ }
    std::cout << "*** inside WinSequences class.  p1GameWinSequence() ***" << std::endl;
    _undo->memory();
    player->setPoints( 0 ); player->getOpponent()->setPoints( 0 );
    std::cout << "*** SetWin::execute() ***" << std::endl;
    _undo->memory();
    _setLeds->updateSets();
    std::cout << "updating set leds" << std::endl;
    _setLeds->updateSets(); 
    std::cout << "*** end of SetWin::execute() ***" << std::endl; }