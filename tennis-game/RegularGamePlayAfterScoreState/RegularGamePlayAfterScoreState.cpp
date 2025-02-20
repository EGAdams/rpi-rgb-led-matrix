#include "RegularGamePlayAfterScoreState.h"

void RegularGamePlayAfterScoreState::handleInput( RemoteListenerContext& context ) {
    context.lock();  // Ensure thread safety

    // print( "===============================================" );
    // print( "=== [STATE: RegularGamePlayAfterScoreState] ===" );
    // print( "===============================================\n\n\n" );
    // print( "*** updating scoreboard ***" );
    context.getScoreboard()->update();
    // print( "*** done updating scoreboard ***" );
    
    print( "setting the no blink timer for regular game play after score state..." );
    context.getNoBlinkInputWithTimer()->setTimeout( REGULAR_GAME_PLAY_INPUT_TIMEOUT );
    int selection = context.getNoBlinkInputWithTimer()->getInput(); // Use noBlinkInputWithTimer to detect inactivity
    // ??!!! int selection = context.getGameInput()->getInput(); // // Use noBlinkInputWithTimer to detect inactivity
    if ( selection != 15 ) {
        print( "Selection from gameInput: " + std::to_string( selection ));
    }
    
    if ( selection == 0 ) {
        // print( "*** Invalid selection (0)! ***" );
        context.unlock();
        return;
    }

    if ( selection == INPUT_TIMEOUT_CODE ) {
        print( "*** No Remotes Detected Timeout! Going to sleep mode... ***"    );
        context.getGameState()->setCurrentAction( SLEEP_MODE                    );
        context.getGameState()->setState(         REGULAR_PLAY_SLEEP_STATE      );
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
    if ( selection == GREEN_REMOTE_GREEN_SCORE  ||
         selection == GREEN_REMOTE_RED_SCORE    ||
         selection == RED_REMOTE_GREEN_SCORE    ||
         selection == RED_REMOTE_RED_SCORE      ||
         selection == UMPIRE_REMOTE_GREEN_SCORE ||
         selection == UMPIRE_REMOTE_RED_SCORE
        ) {
        if ( selection == GREEN_REMOTE_GREEN_SCORE || selection == RED_REMOTE_GREEN_SCORE || selection == UMPIRE_REMOTE_GREEN_SCORE ) {
            // print( "***************************" );
            // print( "*** Green player scored ***" );
            // print( "***************************\n\n\n" );
            selection = 1; // represent GREEN
        } else if ( selection == GREEN_REMOTE_RED_SCORE || selection == RED_REMOTE_RED_SCORE || selection == UMPIRE_REMOTE_RED_SCORE ) {
            // print( "*************************" );
            // print( "*** Red player scored ***" );
            // print( "*************************\n\n\n" );
            selection = 2; // represent RED
        }
        context.getGameObject()->playerScore( selection );
    } else if ( selection == GREEN_REMOTE_UNDO || selection == RED_REMOTE_UNDO || selection == UMPIRE_REMOTE_UNDO ) {
        print( "************" );
        print( "*** Undo ***" );
        print( "************\n\n\n" );
        context.getGameObject()->undo();
    } else {
        // print( "*** Invalid selection ***" );
        // showHelp();
    }

    std::this_thread::sleep_for( std::chrono::milliseconds( SCORE_DELAY_IN_MILLISECONDS ));
    context.getGameObject()->loopGame();
    context.unlock();
}

void RegularGamePlayAfterScoreState::showHelp() {
    print( "------------" );
    print( "GREEN REMOTE: " );
    print( "   green remote green score: "  <<  GREEN_REMOTE_GREEN_SCORE );
    print( "or green remote, red score: "   << GREEN_REMOTE_RED_SCORE );
    print( "or green remote, undo: "        << GREEN_REMOTE_UNDO );
    print( " ------------ \n");
    print( "RED REMOTE: " );
    print( "or red remote, green score: "   << RED_REMOTE_GREEN_SCORE );
    print( "or red remote, red score: "     << RED_REMOTE_RED_SCORE );
    print( "or red remote, undo: "          << RED_REMOTE_UNDO );
    print( " ------------ \n");
    print( "UMPIRE REMOTE: " );
    print( "or umpire remote, green score: " << UMPIRE_REMOTE_GREEN_SCORE );
    print( "or umpire remote, red score: "   << UMPIRE_REMOTE_RED_SCORE   );
    print( "or umpire remote, undo: "        << UMPIRE_REMOTE_UNDO );
    print( " ------------ \n");
}
