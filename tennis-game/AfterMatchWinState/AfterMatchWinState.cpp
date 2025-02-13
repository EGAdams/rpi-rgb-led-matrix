/*
 * After Match Win State
 */
#include "AfterMatchWinState.h"

void AfterMatchWinState::handleInput( RemoteListenerContext& context ) {
    context.lock();  // <--------------------------- lock the context --------------------------<<
    print( "===================================" );
    print( "=== [STATE: AfterMatchWinState] ===" );
    print( "===================================\n\n\n" );
    int selection = context.getNoBlinkInputWithTimer()->getInput(); // Use noBlinkInputWithTimer to detect inactivity
    print( "Selection from noBlinkInputWithTimer: " + std::to_string( selection ));

    if ( selection == INPUT_TIMEOUT_CODE ) {
        print( "*** After Match Win Timeout! Going to sleep mode... ***" );
        context.getGameObject()->resetMatch();
        print( "done resetting match." );
        print( "clearing History because max sleep time has been reached or exceeded." );
        context.getGameObject()->getHistory()->clearHistory();
        print( "done clearing history because max sleep time has been reached or exceeded." );
        print( "setting action to SLEEP_MODE and state to NO_SCORE_SLEEP_STATE..." );
        context.getGameState()->setCurrentAction(   SLEEP_MODE           );
        context.getGameState()->setState(           NO_SCORE_SLEEP_STATE );
        context.unlock();
        return;
    } else if ( selection == GREEN_REMOTE_GREEN_SCORE   ||
                selection == GREEN_REMOTE_RED_SCORE     ||
                selection == RED_REMOTE_GREEN_SCORE     ||
                selection == RED_REMOTE_RED_SCORE ) {
        print( "*** Green player button pressed, resetting match... ***" );
        context.getGameObject()->resetMatch();
        print( "updating scoreboard..." );
        context.getScoreboard()->update();
        print( "setting state to REGULAR_PLAY_NO_SCORE_STATE..." );
        context.getGameState()->setState( REGULAR_PLAY_NO_SCORE_STATE );
        print( "unlocking context and returning..." );
        context.unlock();
        return;
    } else if ( selection == GREEN_REMOTE_UNDO || selection == RED_REMOTE_UNDO ) {
        print( "*** Undo ***" );
        context.getGameObject()->undo();
        print( "setting state to REGULAR_PLAY_AFTER_SCORE_STATE..." );
        context.getGameState()->setState( REGULAR_PLAY_AFTER_SCORE_STATE );
        context.unlock();
        return;
   } else {
        print( "*** Invalid input, unlocking context and returning... ***" );
        // context.getGameObject()->resetMatch();
        // print( "updating scoreboard..." );
        // context.getScoreboard()->update();
        // print( "setting state to REGULAR_PLAY_NO_SCORE_STATE..." );
        // context.getGameState()->setState( NO_SCORE_SLEEP_STATE );
        context.unlock();
        return;
   }                                            
}
