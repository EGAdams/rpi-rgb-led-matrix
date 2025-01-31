#include <thread>
#include <chrono>
#include <iostream>
#include "../StateMachine/StateMachine.h"
#include "../GameState/GameState.h"
#include "../TennisConstants/TennisConstants.h" // Assuming this contains SCORE_DELAY, PAIRING_MODE, etc.

/**
 * @brief Constructs the StateMachine and initializes the starting state.
 * @param context Reference to the RemoteListenerContext.
 */
StateMachine::StateMachine( RemoteListenerContext& context )
    : _context( context ) {
    setState( PAIRING_MODE ); // Start in Pairing Mode
}

/**
 * @brief Runs the state machine loop, processing input and transitioning states.
 */
void StateMachine::run() {
    while ( _context.gameState->gameRunning() && gSignalStatus != SIGINT ) {
        std::this_thread::sleep_for( std::chrono::seconds( SCORE_DELAY ) );

        // Execute the logic for the current state
        _currentState->handleInput( _context );

        // Check for state transition
        int nextState = _context.gameState->getCurrentAction();
        if ( nextState != _currentStateId ) {
            setState( nextState );
        }
    }
}

/**
 * @brief Transitions the state machine to a new state.
 * @param newState The new state identifier.
 */
void StateMachine::setState( int newState ) {
    _currentStateId = newState;                 // set to  SLEEP_MODE, 
    _currentState = createState( newState );    // PAIRING_MODE, etc.
}

/**
 * @brief Creates the appropriate state object based on the game action.
 * @param currentAction The action representing the current state.
 * @return A unique pointer to the corresponding state object.
 */
std::unique_ptr<IRemoteListenerState> StateMachine::createState( int currentAction ) {
    switch ( currentAction ) {
    case PAIRING_MODE:
        return std::make_unique<PairingModeState>();
    case SLEEP_MODE:
        return std::make_unique<SleepModeState>();
    case REGULAR_PLAY_NO_SCORE:
        return std::make_unique<RegularGamePlayBeforeScoreState>();
    case REGULAR_PLAY_AFTER_SCORE:
    case AFTER_SLEEP_MODE: // Treat "After Sleep" similar to "After Score"
        return std::make_unique<RegularGamePlayAfterScoreState>();
    default:
        std::cerr << "*** WARNING: Unknown state, defaulting to RegularGamePlayAfterScore ***" << std::endl;
        return std::make_unique<RegularGamePlayAfterScoreState>();
    }
}
