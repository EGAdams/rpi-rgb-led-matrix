#include "PairingModeState.h"

/***************************************************************
 * PairingModeState::handleInput()
 *
 * - Listens for player inputs to pair players.
 * - If a valid input is received, marks the corresponding player as paired.
 * - If both players are paired or pairing times out, transitions to sleep mode.
 ***************************************************************/
void PairingModeState::handleInput( RemoteListenerContext& context ) {
    context.lock();  // Ensure thread safety
    print( "============================" );
    print( "=== [STATE: PairingMode] ===" );
    print( "============================\n" );

    // **Null Safety Checks**
    if ( !context.getRemotePairingScreen()) {
        print( "*** ERROR: remotePairingScreen is NULL! ***" );
        context.unlock();
        return;
    }
    if ( !context.getPairingBlinker()) {
        print( "*** ERROR: pairingBlinker is NULL! ***" );
        context.unlock();
        return;
    } 
    if ( !context.getPairingInputWithTimer()) {
        print( "*** ERROR: pairingInputWithTimer is NULL! ***" );
        context.unlock();
        return;
    }

    // Ensure we are in pairing mode and the blinker is active
    while ( context.getRemotePairingScreen()->inPairingMode() && context.getPairingBlinker()->awake()) {
        print( "Waiting for pairing input..." );
        int selection = context.getPairingInputWithTimer()->getInput(); // the one with the pairing blinker
        if ( selection == GREEN_REMOTE_GREEN_SCORE ) {
            context.getRemotePairingScreen()->greenPlayerPressed();
            context.getPairingBlinker()->setGreenPlayerPaired( true );
            print( "Green player paired." );
        } else if ( selection == RED_REMOTE_RED_SCORE ) {
            context.getRemotePairingScreen()->redPlayerPressed();
            context.getPairingBlinker()->setRedPlayerPaired( true );
            print( "Red player paired." );
        } else if ( selection == INPUT_TIMEOUT_CODE ) {
            print( "Pairing timeout. Transitioning to sleep mode..." );
            context.getGameState()->setCurrentAction( SLEEP_MODE );
            context.getGameState()->setState( PAIRING_SLEEP_MODE_STATE );
            context.unlock();
            return;
        } else {
            print( "*** Invalid selection during pairing. ***" );
            std::this_thread::sleep_for( std::chrono::milliseconds( 500 ));
        }
        if ( context.getPairingBlinker()->areBothPlayersPaired()) { // If both players are paired, exit the loop
            print( "Both players paired, exiting pairing mode." );
            context.getGameState()->setState( REGULAR_PLAY_NO_SCORE_STATE );
            break;
        }
    }
    if ( !context.getPairingBlinker()->awake()) {                           // If pairing is complete or timeout 
        print( "Pairing blinker is not awake, stopping pairing mode..." );  // occurs, transition to sleep mode
        context.getPairingBlinker()->stop();
        print( "Transitioning to sleep mode..." );
        context.getGameState()->setCurrentAction( SLEEP_MODE );
        context.getGameState()->setState( PAIRING_SLEEP_MODE_STATE );
    }
    context.unlock(); // Unlock before exiting
}
