/*
 * Zero score timeout state
 */
#include "AfterMatchWinState.h"

void AfterMatchWinState::handleInput( RemoteListenerContext& context ) {
    context.lock();  // Ensure thread safety
    print( "===================================" );
    print( "=== [STATE: AfterMatchWinState] ===" );
    print( "===================================\n\n\n" );
    context.getScoreboard()->update();
    int selection = context.getNoBlinkInputWithTimer()->getInput(); // Use noBlinkInputWithTimer to detect inactivity
    print( "Selection from noBlinkInputWithTimer: " + std::to_string( selection ) );

    if ( selection == INPUT_TIMEOUT_CODE ) {
        print( "*** Zero Score Timeout! Going to sleep mode... ***" );
        context.getGameState()->setCurrentAction( SLEEP_MODE    );
        context.getGameState()->setState( NO_SCORE_SLEEP_STATE  );
        context.unlock();
        return;
    }

    bool& no_score_flag = context.getNoScoreFlag(); // We've received a valid input, so the 
    no_score_flag = false;                          // "no score" condition is no longer valid
    // maybe not h
    // handleSelectionAndUpdate( context, selection ); // Handle the selection and update game state accordingly
    context.getGameState()->setState( REGULAR_PLAY_NO_SCORE_STATE ); // After a valid score input,
    context.unlock();                                                   // transition to REGULAR_PLAY_AFTER_SCORE
}

void AfterMatchWinState::handleSelectionAndUpdate( RemoteListenerContext& context, int selection ) {
    if ( selection == 0 ) {
        print( "*** Invalid selection ( 0 )! ***" );
        return;
    }

    // Check if the correct player is serving
    int serve_flag = context.getRemoteLocker()->playerNotServing( selection );
    print( "*** serve_flag: " + std::to_string( serve_flag ) + " ***" );
    if ( serve_flag ) {
        print( "*** Warning: player not serving! ***" );
        return;
    }

    // If a valid scoring button is pressed
    if ( selection == GREEN_REMOTE_GREEN_SCORE  ||
         selection == GREEN_REMOTE_RED_SCORE    ||
         selection == RED_REMOTE_GREEN_SCORE    ||
         selection == RED_REMOTE_RED_SCORE      ||
         selection == RED_REMOTE_UNDO           ||
         selection == GREEN_REMOTE_UNDO ) {
        if ( selection == GREEN_REMOTE_GREEN_SCORE || selection == RED_REMOTE_GREEN_SCORE ) {
            print( "*** Green player scored ***" );
            selection = 1; // Represent GREEN
        } else if ( selection == GREEN_REMOTE_RED_SCORE || selection == RED_REMOTE_RED_SCORE ) {
            print( "*** Red player scored ***" );
            selection = 2; // Represent RED
        }
        context.getGameObject()->playerScore( selection );
        print( "after setting player score in game object." );
    } else if ( selection == GREEN_REMOTE_UNDO || selection == RED_REMOTE_UNDO ) {
        print( "*** Undo ***" );
        context.getGameObject()->undo();
    } else {
        print( "*** Invalid selection ***" );
        showHelp();
    }

    // Let the GameObject handle updates
    std::this_thread::sleep_for( std::chrono::milliseconds( SCORE_DELAY_IN_MILLISECONDS ));
    context.getGameObject()->loopGame();
}

void AfterMatchWinState::showHelp() {
    print( "------------" );
    print( "GREEN REMOTE:" );
    print( "   Green remote green score: " + std::to_string( GREEN_REMOTE_GREEN_SCORE ) );
    print( "   Green remote red score:   " + std::to_string( GREEN_REMOTE_RED_SCORE ) );
    print( "RED REMOTE:" );
    print( "   Red remote green score:   " + std::to_string( RED_REMOTE_GREEN_SCORE ) );
    print( "   Red remote red score:     " + std::to_string( RED_REMOTE_RED_SCORE ) );
    print( "------------" );
}
