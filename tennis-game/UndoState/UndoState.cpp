#include "UndoState.h"

void UndoState::handleInput( RemoteListenerContext& context ) {
    context.lock();  // Ensure thread safety
    print( "===============================================" );
    print( "=== [STATE: UndoState] ===" );
    print( "===============================================\n\n\n" );
    // print( "*** updating scoreboard ***" );
    context.getScoreboard()->update();   
    print( "setting the no blink timer for the Undo state..." );
    context.getNoBlinkInputWithTimer()->setTimeout( UNDO_STATE_TIMEOUT );
    int selection = context.getNoBlinkInputWithTimer()->getInput(); // Use noBlinkInputWithTimer to detect inactivity
    if ( selection != 15 ) { print( "Selection from gameInput: " + std::to_string( selection ));} // no print for idle

    if ( selection == INPUT_TIMEOUT_CODE ) {
        print( "*** Temporary Undo superpowers invoke, changing back to regular game state... ***"    );
        context.getGameState()->setState( REGULAR_PLAY_AFTER_SCORE_STATE );
        context.getNoBlinkInputWithTimer()->setTimeout( REGULAR_GAME_PLAY_INPUT_TIMEOUT );
        context.unlock();
        return;
    }

    if ( selection == GREEN_REMOTE_UNDO || selection == RED_REMOTE_UNDO ) { // ignore serve
        print( "************" );                                            // flag for this state
        print( "*** Undo ***" );
        print( "************\n\n\n" );
        context.getGameObject()->undo();
        std::this_thread::sleep_for( std::chrono::milliseconds( SCORE_DELAY_IN_MILLISECONDS ));
        context.getGameObject()->loopGame();
    } else {
        print( "*** Invalid selection ***" );
        _showHelp();
    }
    context.unlock();
}

void UndoState::_showHelp() {
    print( "-----------------" );
    print( "GREEN REMOTE:" );
    print( "   green remote undo: " + std::to_string( GREEN_REMOTE_UNDO));
    print( "RED REMOTE:" );
    print( "   red remote undo:   " + std::to_string( RED_REMOTE_UNDO ));
    print( "-----------------\n\n" );
}
