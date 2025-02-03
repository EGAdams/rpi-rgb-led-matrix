#include "SleepModeState.h"

void SleepModeState::handleInput( RemoteListenerContext& context ) {
    context.lock();                             // Lock to keep thread safety
    print( "=== [STATE: SleepMode] ===" );
    int selection = context.getSleepingInputWithTimer()->getInput(); // Block for input from sleepingInputWithTimer    
    if ( selection == GREEN_REMOTE_GREEN_SCORE ||   // Check if the user pressed a valid score button during sleep
         selection == GREEN_REMOTE_RED_SCORE   ||
         selection == RED_REMOTE_UNDO          ||
         selection == GREEN_REMOTE_UNDO ) {
        print( "Time slept: " + std::to_string( context.getSleepingInputWithTimer()->getTimeSlept()));
        if ( selection == GREEN_REMOTE_UNDO || selection == RED_REMOTE_UNDO ) {
            print( "Undoing last action..." );
            context.getGameObject()->undo();
        } else {                                // must have it a score button, reset match...
            print( "Resetting match..." );
            context.getGameObject()->resetMatch();
            print( "Match reset complete." );
            if ( context.getSleepingInputWithTimer()->getTimeSlept() > SLEEP_INPUT_TIMEOUT * 1000 ) { // If sleep time exceeded
                print( "Max sleep time exceeded. Clearing game history..." );                         // SLEEP_INPUT_TIMEOUT, clear game history  
                context.getGameObject()->getHistory()->clearHistory();
                print( "Game history cleared." );
            }
        }
        print( "Clearing scoreboard screen..." ); // clean off bouncing ball
        context.getScoreboard()->clearScreen();
        print( "Screen cleared." );
        context.getScoreboard()->update();
        print( "Scoreboard updated." );
        context.getGameState()->setCurrentAction( AFTER_SLEEP_MODE ); // Transition to AFTER_SLEEP_MODE
        context.getGameState()->setState( REGULAR_PLAY_NO_SCORE_STATE);
        context.unlock();  // Unlock before returning
        return;
    }
    print( "*** Waking up from Sleep Mode, resuming last match ***" );
    context.unlock();                           // Unlock after finishing
}
