#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <thread>
#include <chrono>
#include <iostream>
#include <memory>
#include "../IRemoteListenerState/IRemoteListenerState.h"
#include "../PairingModeState/PairingModeState.h"
#include "../SleepModeState/SleepModeState.h"
#include "../RegularGamePlayBeforeScoreState/RegularGamePlayBeforeScoreState.h"
#include "../RegularGamePlayAfterScoreState/RegularGamePlayAfterScoreState.h"
#include "../GameState/GameState.h"
#include "../TennisConstants/TennisConstants.h" // Assuming this contains SCORE_DELAY, PAIRING_MODE, etc.
#include "../AfterMatchWinState/AfterMatchWinState.h"
#include "../MatchWinBlinkState/MatchWinBlinkState.h"
#include "../UndoState/UndoState.h"

/**
 * @class StateMachine
 * @brief Manages the different game states using the State Pattern.
 *
 * The StateMachine class encapsulates:
 *  - The current state of the system
 *  - The logic for transitioning between states
 *  - Execution of the current state's behavior
 */
class StateMachine {
public:
  /**
   * @brief Constructs the StateMachine with the given context.
   * @param context Reference to the RemoteListenerContext.
   */
  explicit StateMachine( RemoteListenerContext& context );

  /**
   * @brief Runs the state machine, executing the current state in a loop.
   */
  void run();

private:
  RemoteListenerContext& _context;
  std::unique_ptr<IRemoteListenerState> _currentState;
  int _currentStateId;
  int _state_machine_delay = 500; // while loop delay in milliseconds

  /**
   * @brief Sets the current state based on the game state.
   * @param newState The new state to transition into.
   */
  void setState( int newState );

  /**
   * @brief Creates the appropriate state object based on the current action.
   * @param currentAction The action representing the current state.
   * @return A unique pointer to the new state object.
   */
  std::unique_ptr<IRemoteListenerState> createState( int currentAction );
};

#endif // STATE_MACHINE_H
