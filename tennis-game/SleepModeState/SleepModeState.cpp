#include "SleepModeState.h"

void SleepModeState::handleInput( RemoteListenerContext& context ) {
    context.lock();                             // Lock to keep thread safety
    print( "==========================" );
    print( "=== [STATE: SleepMode] ===" );
    print( "==========================\n\n\n" );
    int selection = context.getSleepingInputWithTimer()->getInput(); // Block for input from sleepingInputWithTimer    
    if ( selection == GREEN_REMOTE_GREEN_SCORE  ||   // Check if the user pressed a valid score button during sleep
         selection == GREEN_REMOTE_RED_SCORE    ||
         selection == RED_REMOTE_GREEN_SCORE    ||
         selection == RED_REMOTE_RED_SCORE      ||
         selection == UMPIRE_REMOTE_GREEN_SCORE ||
         selection == UMPIRE_REMOTE_RED_SCORE   ||
         selection == RED_REMOTE_UNDO           ||
         selection == GREEN_REMOTE_UNDO         ||
         selection == UMPIRE_REMOTE_UNDO        
        ) {
        print( "Time slept: " + std::to_string( context.getSleepingInputWithTimer()->getTimeSlept()));
        
        print( "if we are sleeping because of no remote activity, then we should wake up in regular play without adjusting the score." );
        if ( context.getGameState()->getState() == REGULAR_PLAY_SLEEP_STATE ) {
            context.getGameState()->setState( REGULAR_PLAY_AFTER_SCORE_STATE ); // Wake up in regular play
            context.unlock();                                                   // Unlock before returning
            return;
        }

        if ( selection == GREEN_REMOTE_UNDO || selection == RED_REMOTE_UNDO || selection == UMPIRE_REMOTE_UNDO) {
            print( "Undoing last action..." );
            context.getGameObject()->undo();
        } else {                                // must have it a score button, reset match...
            print( "not doing anything here except clearing off the bouncing ball and setting the state..." );
            // print( "Resetting match..." );
            // context.getGameObject()->resetMatch();
            // print( "Match reset complete." );
            // if ( context.getSleepingInputWithTimer()->getTimeSlept() > SLEEP_INPUT_TIMEOUT * 1000 ) { // If sleep time exceeded
            //     print( "Max sleep time exceeded. Clearing game history..." );                         // SLEEP_INPUT_TIMEOUT, clear game history  
            //     context.getGameObject()->getHistory()->clearHistory();
            //     print( "Game history cleared." );
            // }
        }
        print( "Clearing scoreboard screen..." );                       // clean off bouncing ball
        context.getScoreboard()->clearScreen();
        print( "Screen cleared." );
        // context.getScoreboard()->update();                           // Shows Zeros!
        // print( "Scoreboard updated." );
        context.getGameState()->setCurrentAction( AFTER_SLEEP_MODE );           // Transition to AFTER_SLEEP_MODE
        if ( context.getGameState()->getState() == NO_SCORE_SLEEP_STATE ) {     
            context.getGameState()->setState( REGULAR_PLAY_NO_SCORE_STATE );    // Wake up in the beginning of the game
        } else if ( context.getGameState()->getState() == PAIRING_SLEEP_MODE_STATE ) {
            context.getGameState()->setState( PAIRING_MODE_STATE );             // Wake up in pairing mode
        }
        context.unlock();  // Unlock before returning
        return;
    }
    print( "*** Warning: Waking up from Sleep Mode with unknown input! ***" );
    context.getGameState()->setCurrentAction( AFTER_SLEEP_MODE );           // Transition to AFTER_SLEEP_MODE
    if ( context.getGameState()->getState() == NO_SCORE_SLEEP_STATE ) {     
        context.getGameState()->setState( REGULAR_PLAY_NO_SCORE_STATE );    // Wake up in the beginning of the game
    } else if ( context.getGameState()->getState() == PAIRING_SLEEP_MODE_STATE ) {
        context.getGameState()->setState( PAIRING_MODE_STATE );             // Wake up in pairing mode
    }
    context.unlock();  // Unlock before returning
    context.unlock();                                                  // Unlock after finishing
}
