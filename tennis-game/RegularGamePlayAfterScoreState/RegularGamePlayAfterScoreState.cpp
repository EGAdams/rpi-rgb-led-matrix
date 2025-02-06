#include "RegularGamePlayAfterScoreState.h"

void RegularGamePlayAfterScoreState::handleInput( RemoteListenerContext& context ) {
    context.lock();  // Ensure thread safety

    print( "=== [STATE: RegularGamePlayAfterScore] ===" );
    print( "*** updating scoreboard ***" );
    context.getScoreboard()->update();
    print( "*** done updating scoreboard ***" );

    // Block on the gameInput (no timers here)
    int selection = context.getGameInput()->getInput();
    print( "Selection from gameInput: " + std::to_string( selection ) );
    if ( selection == 0 ) {
        print( "*** Invalid selection (0)! ***" );
        context.unlock();
        return;
    }

    // Check if the correct server is pressed
    int serve_flag = context.getRemoteLocker()->playerNotServing( selection );
    print( "*** serve_flag: " + std::to_string( serve_flag ) + " ***" );
    if ( serve_flag ) {
        print( "*** Warning: player not serving! ***" );
        context.unlock();
        return;
    }

    // If a valid scoring button is pressed
    if ( selection == GREEN_REMOTE_GREEN_SCORE ||
        selection == GREEN_REMOTE_RED_SCORE ||
        selection == RED_REMOTE_GREEN_SCORE ||
        selection == RED_REMOTE_RED_SCORE ) {
        if ( selection == GREEN_REMOTE_GREEN_SCORE || selection == RED_REMOTE_GREEN_SCORE ) {
            print( "*** Green player scored ***" );
            selection = 1; // represent GREEN
        } else if ( selection == GREEN_REMOTE_RED_SCORE || selection == RED_REMOTE_RED_SCORE ) {
            print( "*** Red player scored ***" );
            selection = 2; // represent RED
        }
        context.getGameObject()->playerScore( selection );
    } else if ( selection == GREEN_REMOTE_UNDO || selection == RED_REMOTE_UNDO ) {
        print( "*** Undo ***" );
        context.getGameObject()->undo();
    } else {
        print( "*** Invalid selection ***" );
        showHelp();
    }

    std::this_thread::sleep_for( std::chrono::milliseconds( SCORE_DELAY_IN_MILLISECONDS ) );
    context.getGameObject()->loopGame();
    context.unlock();
}

void RegularGamePlayAfterScoreState::showHelp() {
    print( "------------" );
    print( "GREEN REMOTE:" );
    print( "   green remote green score: " + std::to_string( GREEN_REMOTE_GREEN_SCORE ) );
    print( "   green remote red score:   " + std::to_string( GREEN_REMOTE_RED_SCORE ) );
    print( "RED REMOTE:" );
    print( "   red remote green score:   " + std::to_string( RED_REMOTE_GREEN_SCORE ) );
    print( "   red remote red score:     " + std::to_string( RED_REMOTE_RED_SCORE ) );
    print( "------------" );
}
