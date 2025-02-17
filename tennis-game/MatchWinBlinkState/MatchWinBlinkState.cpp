#include "MatchWinBlinkState.h"
#include "../RemoteListenerContext/RemoteListenerContext.h"
#include "../GameState/GameState.h"
#include "../GameObject/GameObject.h"
#include "../Inputs/Inputs.h"
#include "../History/History.h"
#include "../TennisConstants/TennisConstants.h"
#define MATCH_WIN_OFFSET 12

MatchWinBlinkState::MatchWinBlinkState() {}

void MatchWinBlinkState::handleInput( RemoteListenerContext& context ) {
    context.lock();
    print( "===================================" );
    print( "=== [STATE: MatchWinBlinkState] ===" );
    print( "===================================\n\n\n" );
    startBlinking( context );                                             // Start blinking asynchronously
    unsigned long original_no_blink_time    = MAIN_INPUT_TIMEOUT;
    unsigned long match_blinking_time       = 2 * MATCH_WIN_FLASH_DELAY * MATCH_WIN_BLINK_COUNT + 100 /*ms*/; // change the timeout  
    context.getNoBlinkInputWithTimer()->setTimeout( match_blinking_time );  // of the no blink timer to the match
    while ( context.getMatchWinBlinker()->isRunning() ) {                   // blinking time + 1 second for a buffer
        int selection = context.getNoBlinkInputWithTimer()->getInput();     // Listen for input while blinking
        print( "Selection from noBlinkInputWithTimer: " + std::to_string( selection ) );
        if ( selection == GREEN_REMOTE_GREEN_SCORE  ||
            selection == GREEN_REMOTE_RED_SCORE     ||
            selection == RED_REMOTE_GREEN_SCORE     ||
            selection == RED_REMOTE_RED_SCORE ) {
            print( "*** Player button pressed, ignore during Match Win Blink State... ***" );
            // context.getGameState()->setState( REGULAR_PLAY_AFTER_SCORE_STATE );
            // context.getScoreboard()->update();
            // context.getMatchWinBlinker()->stop();  // Stop blinking early
            // context.getNoBlinkInputWithTimer()->setTimeout( original_no_blink_time );   // reset the main input 
            // context.unlock();                                                           // timeout of the no blink
            // return;                                                                     // timer before returning.
            continue;
        } else if ( selection == GREEN_REMOTE_UNDO || selection == RED_REMOTE_UNDO ) {
            print( "*** Undo ***" );
            context.getGameObject()->undo();
            context.getGameState()->setState( REGULAR_PLAY_AFTER_SCORE_STATE );
            context.getScoreboard()->update();
            context.getMatchWinBlinker()->stop(); // Stop blinking early
            context.getNoBlinkInputWithTimer()->setTimeout( original_no_blink_time );   // reset the main input 
            context.unlock();                                                           // timeout of no blink
            return;                                                                     // timeout before returning.
        }
    }
    print( "*** MatchWinBlinker finished blinking, transitioning to AfterMatchWinState ***" ); // No input. 
    context.getGameState()->setState( AFTER_MATCH_WIN_STATE );                                 // Blinkingcomplete.
    context.getScoreboard()->clearScreen();          // pressing score after breaking out of Match Win Blink State
    context.getScoreboard()->drawNewText( "Match", 13, 60 - MATCH_WIN_OFFSET );
    context.getScoreboard()->drawNewText( "Win", 20, 75 - MATCH_WIN_OFFSET );
    context.getScoreboard()->drawSets();
    context.getNoBlinkInputWithTimer()->setTimeout( original_no_blink_time ); // reset the main input blink time
    context.unlock();
}

void MatchWinBlinkState::startBlinking( RemoteListenerContext& context ) {
    if ( !context.getMatchWinBlinker()->isRunning() ) {
        context.getMatchWinBlinker()->start();
    }
}
