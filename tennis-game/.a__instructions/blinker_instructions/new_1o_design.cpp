/************************************************************
 * run_remote_listener.cpp
 *
 * Demonstration of using a State Pattern to manage the
 * "beginning game states" for a tennis scoring system.
 *
 * The goal is to make the code more modular, testable,
 * maintainable, and thread-safe.
 *
 * The four primary states that we care about are:
 *   1) Pairing Mode
 *   2) Sleep Mode
 *   3) Regular Game Play (Before any Scores)
 *   4) Regular Game Play (After a Score)
 *
 * This file shows a re-organization of logic using the
 * GoF State Pattern. Each state is implemented as a class
 * responsible for its own behavior and for performing
 * state transitions as needed.
 *
 * The run_remote_listener() function is refactored to:
 *   - Initialize all shared resources (GameObject, Inputs, etc.)
 *   - Create a Context (InputListenerContext) which holds
 *     references to the shared resources, input readers,
 *     scoreboard, blinkers, etc.
 *   - Loop until the game is no longer running or receives SIGINT
 *   - Delegate game logic to the current State object
 *   - The State object can transition to a different State by
 *     updating the "currentAction" in the shared GameState or
 *     by calling a helper method in the Context.
 *
 * NOTE: This code block focuses ONLY on rewriting the
 * run_remote_listener.cpp logic. It references classes
 * and methods you already have (such as GameObject, IInputWithTimer,
 * Blinker, etc.) but does not redefine them here. 
 * For any new interfaces introduced (e.g. IInputListenerState),
 * we put them at the top of this file for illustration.
 *
 * IMPORTANT: Thread-safety
 * -----------
 * This example adds a few std::mutex locks around sensitive
 * operations to guard shared resources (e.g. scoreboard, gameObject).
 * Actual concurrency considerations may require further locking
 * depending on how scoreboard, gameObject, etc. are implemented.
 ************************************************************/

#include <iostream>
#include <thread>
#include <chrono>
#include <csignal>
#include <map>
#include <mutex>
#include <memory>

// ---------------------------------------------------------
// Forward declarations of existing classes/interfaces
// ---------------------------------------------------------
class GameState;
class GameObject;
class Reset;
class Inputs;
class RemoteLocker;
class IInputWithTimer;
class IGameInput;
class Blinker;
class ScoreBoard;
class PairingBlinker;
class BlankBlinker;
class ScoreboardBlinker;
class RemotePairingScreen;

// External global for receiving Ctrl-C signals, etc.
extern volatile std::sig_atomic_t gSignalStatus;

// Constants representing input from the remote
// You presumably have these defined elsewhere already.
static const int GREEN_REMOTE_GREEN_SCORE = 100;
static const int GREEN_REMOTE_RED_SCORE   = 101;
static const int RED_REMOTE_GREEN_SCORE   = 102;
static const int RED_REMOTE_RED_SCORE     = 103;
static const int GREEN_REMOTE_UNDO        = 104;
static const int RED_REMOTE_UNDO          = 105;

// Some additional constants for game states. 
// They presumably come from an enum or #define in your code.
static const int PAIRING_MODE    = 1;
static const int SLEEP_MODE      = 2;
static const int AFTER_SLEEP_MODE= 3;   // e.g. "wake up" mode
static const int REGULAR_PLAY_NO_SCORE    = 4; 
static const int REGULAR_PLAY_AFTER_SCORE = 5; 

// For demonstration, used in the loop
static const int SCORE_DELAY     = 1;  // number of seconds to wait
static const unsigned long MAX_SLEEP = 300; // just an example

// You presumably have some print function
static void print(const std::string& msg) { 
    std::cout << msg << std::endl; 
}

/***************************************************************
 * IInputListenerState (State Pattern Interface)
 * 
 * Each concrete state will implement handleInput() (or 
 * handleState()) which will:
 *   - Read input from the appropriate IInputWithTimer or IGameInput
 *   - Update shared data (gameObject, scoreboard, gameState, etc.)
 *   - Potentially transition to another state by setting 
 *     gameState->setCurrentAction(...)
 ***************************************************************/
class InputListenerContext;  // forward declaration

class IInputListenerState {
public:
    virtual ~IInputListenerState() = default;
    virtual void handleInput(InputListenerContext &context) = 0;
};

/***************************************************************
 * InputListenerContext
 *
 * This holds all the shared resources the states need:
 *   - references/pointers to gameObject, scoreboard, blinkers,
 *     input timers, and so on
 *   - a mechanism for locking/unlocking shared resources
 ***************************************************************/
class InputListenerContext {
public:
    // Constructor initializes references and pointers
    InputListenerContext(GameObject*  gameObj,
                          GameState*   gameSt,
                          Reset*       resetPtr,
                          RemoteLocker* rLocker,
                          IInputWithTimer* pairingTimer,
                          IInputWithTimer* noBlinkTimer,
                          IInputWithTimer* sleepingTimer,
                          IGameInput*      gameIn,
                          RemotePairingScreen* pairingScreen,
                          PairingBlinker*  pairingBlinker,
                          BlankBlinker*    blankBlinker,
                          ScoreboardBlinker* sleepingBlinker,
                          bool& noScoreFlag)
    : gameObject(gameObj),
      gameState(gameSt),
      reset(resetPtr),
      remoteLocker(rLocker),
      pairingInputWithTimer(pairingTimer),
      noBlinkInputWithTimer(noBlinkTimer),
      sleepingInputWithTimer(sleepingTimer),
      gameInput(gameIn),
      remotePairingScreen(pairingScreen),
      pairingBlinkerObj(pairingBlinker),
      blankBlinkerObj(blankBlinker),
      sleepingBlinkerObj(sleepingBlinker),
      no_score(noScoreFlag)
    {
        scoreboard = gameObject->getScoreBoard();
    }

    // A mutex to protect shared resources as needed
    std::mutex mtx;

    // Pointers to major game objects
    GameObject*          gameObject;
    GameState*           gameState;
    Reset*               reset;
    RemoteLocker*        remoteLocker;
    IInputWithTimer*     pairingInputWithTimer;
    IInputWithTimer*     noBlinkInputWithTimer;
    IInputWithTimer*     sleepingInputWithTimer;
    IGameInput*          gameInput;
    RemotePairingScreen* remotePairingScreen;

    // Blinkers
    PairingBlinker*      pairingBlinkerObj;
    BlankBlinker*        blankBlinkerObj;
    ScoreboardBlinker*   sleepingBlinkerObj;

    // Scoreboard is often used in the states
    ScoreBoard*          scoreboard;

    // Additional flags
    bool& no_score;  // reference to the "no_score" bool
};


/***************************************************************
 * PairingModeState
 * Handles logic specific to pairing (PairingBlinker, etc.)
 ***************************************************************/
class PairingModeState : public IInputListenerState {
public:
    void handleInput(InputListenerContext &context) override {
        // Lock shared resources if necessary
        std::lock_guard<std::mutex> lock(context.mtx);

        print("=== [STATE: PairingMode] ===");
        
        // The original code checks if we are "inPairingMode" 
        // AND if the pairing blinker is "awake"
        while (context.remotePairingScreen->inPairingMode() 
               && context.pairingBlinkerObj->awake()) 
        {
            print("Inside remote pairing mode - waiting for pairing input...");
            int selection = context.pairingInputWithTimer->getInput();
            if (selection == GREEN_REMOTE_GREEN_SCORE) {
                context.remotePairingScreen->greenPlayerPressed();
                context.pairingBlinkerObj->setGreenPlayerPaired(true);
            } 
            else if (selection == RED_REMOTE_RED_SCORE) {
                context.remotePairingScreen->redPlayerPressed();
                context.pairingBlinkerObj->setRedPlayerPaired(true);
            } 
            else {
                print("*** Invalid selection during pairing. *** 012525");
                // Sleep a bit
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }
        }

        // If pairingBlinker is no longer awake, we transition to SLEEP_MODE
        if (!context.pairingBlinkerObj->awake()) {
            print("pairing blinker is not awake, stopping it...");
            context.pairingBlinkerObj->stop();
            print("pairing blinker stopped.  now putting in sleep mode...");
            context.gameState->setCurrentAction(SLEEP_MODE);
        }
    }
};

/***************************************************************
 * SleepModeState
 * Blocks input for a certain time, then transitions to AFTER_SLEEP_MODE
 ***************************************************************/
class SleepModeState : public IInputListenerState {
public:
    void handleInput(InputListenerContext &context) override {
        std::lock_guard<std::mutex> lock(context.mtx);

        print("=== [STATE: SleepMode] ===");

        // We block on the sleepingInputWithTimer call for so many seconds
        int selection = context.sleepingInputWithTimer->getInput();
        // After sleeping, transition to AFTER_SLEEP_MODE
        context.gameState->setCurrentAction(AFTER_SLEEP_MODE);

        print("time slept: " + std::to_string(context.sleepingInputWithTimer->getTimeSlept()));
        
        // If the user pressed a Green or Red "score" button during sleep:
        if (selection == GREEN_REMOTE_GREEN_SCORE || selection == GREEN_REMOTE_RED_SCORE) {
            print("reset match.");
            context.gameObject->resetMatch();
            print("done resetting match.");

            if (context.sleepingInputWithTimer->getTimeSlept() > MAX_SLEEP * 1000) {
                print("time slept: " 
                      + std::to_string(context.sleepingInputWithTimer->getTimeSlept() / 1000) 
                      + " seconds. clearing history due to max sleep time.");
                context.gameObject->getHistory()->clearHistory();
            }
            // We remain in the loop, 
            // next iteration of while() in run_remote_listener will pick state again
            return;
        }
        
        print("*** Going into last Match! ***");
        print("clearing screen...");
        context.scoreboard->clearScreen();
        print("cleared screen.");
        context.scoreboard->update();
        print("updated scoreboard.");
    }
};

/***************************************************************
 * RegularGamePlayBeforeScoreState
 * This is the state for normal game play, specifically before
 * any real score has occurred. We have an input timer to watch
 * for inactivity (leading to SleepMode).
 ***************************************************************/
class RegularGamePlayBeforeScoreState : public IInputListenerState {
public:
    void handleInput(InputListenerContext &context) override {
        std::lock_guard<std::mutex> lock(context.mtx);

        print("=== [STATE: RegularGamePlayBeforeScore] ===");

        // Use the noBlinkInputWithTimer to detect "no activity"
        int selection = context.noBlinkInputWithTimer->getInput();
        print("selection from noBlinkInputWithTimer: " + std::to_string(selection));
        
        // If timed out, selection is likely > 1000 (or some sentinel)
        if (selection > 1000) {
            print("*** Zero ScoreTimeout!  going to sleep mode... ***");
            context.gameState->setCurrentAction(SLEEP_MODE);
            return;
        }

        // We've received a valid input or something else
        // So there's now a "score" in the system. 
        // Let's mark no_score = false so we don't do the 
        // noBlink timeout logic next time.
        context.no_score = false;

        // We haven't actually updated the game yet. We'll do that
        // in the "AfterScore" logic or handle it directly here:
        handleSelectionAndUpdate(context, selection);

        // After the first valid score input, let's go to the "after score" state
        context.gameState->setCurrentAction(REGULAR_PLAY_AFTER_SCORE);
    }

private:
    // Helper to handle the actual game scoring logic
    void handleSelectionAndUpdate(InputListenerContext &context, int selection) {
        if (selection == 0) {
            print("*** Invalid selection (0)! ***");
            return;
        }

        // Check if the correct server is pressed
        int serve_flag = context.remoteLocker->playerNotServing(selection);
        print("*** serve_flag: " + std::to_string(serve_flag) + " ***");
        if (serve_flag) {
            print("*** Warning: player not serving! ***");
            return;
        }

        // If a valid scoring button is pressed
        if (selection == GREEN_REMOTE_GREEN_SCORE ||
            selection == GREEN_REMOTE_RED_SCORE   ||
            selection == RED_REMOTE_GREEN_SCORE   ||
            selection == RED_REMOTE_RED_SCORE) 
        {
            if (selection == GREEN_REMOTE_GREEN_SCORE || selection == RED_REMOTE_GREEN_SCORE) {
                print("*** Green player scored ***");
                selection = 1; // represent GREEN
            } 
            else if (selection == GREEN_REMOTE_RED_SCORE || selection == RED_REMOTE_RED_SCORE) {
                print("*** Red player scored ***");
                selection = 2; // represent RED
            }
            context.gameObject->playerScore(selection);
        }
        else if (selection == GREEN_REMOTE_UNDO || selection == RED_REMOTE_UNDO) {
            print("*** Undo ***");
            context.gameObject->undo();
        }
        else {
            print("*** Invalid selection ***");
            showHelp();
        }

        // Let the gameObject handle updates
        std::this_thread::sleep_for(std::chrono::seconds(SCORE_DELAY));
        context.gameObject->loopGame();
    }

    void showHelp() {
        print("------------");
        print("GREEN REMOTE:");
        print("   green remote green score: " + std::to_string(GREEN_REMOTE_GREEN_SCORE));
        print("   green remote red score:   " + std::to_string(GREEN_REMOTE_RED_SCORE));
        print("RED REMOTE:");
        print("   red remote green score:   " + std::to_string(RED_REMOTE_GREEN_SCORE));
        print("   red remote red score:     " + std::to_string(RED_REMOTE_RED_SCORE));
        print("------------");
    }
};

/***************************************************************
 * RegularGamePlayAfterScoreState
 * Very similar logic to "before score", but no timer for 
 * "no activity." 
 ***************************************************************/
class RegularGamePlayAfterScoreState : public IInputListenerState {
public:
    void handleInput(InputListenerContext &context) override {
        std::lock_guard<std::mutex> lock(context.mtx);

        print("=== [STATE: RegularGamePlayAfterScore] ===");

        // We simply block on the gameInput (no timers here)
        int selection = context.gameInput->getInput();
        if (selection == 0) {
            print("*** Invalid selection (0)! ***");
            return;
        }

        // Check if the correct server is pressed
        int serve_flag = context.remoteLocker->playerNotServing(selection);
        print("*** serve_flag: " + std::to_string(serve_flag) + " ***");
        if (serve_flag) {
            print("*** Warning: player not serving! ***");
            return;
        }

        // If a valid scoring button is pressed
        if (selection == GREEN_REMOTE_GREEN_SCORE ||
            selection == GREEN_REMOTE_RED_SCORE   ||
            selection == RED_REMOTE_GREEN_SCORE   ||
            selection == RED_REMOTE_RED_SCORE) 
        {
            if (selection == GREEN_REMOTE_GREEN_SCORE || selection == RED_REMOTE_GREEN_SCORE) {
                print("*** Green player scored ***");
                selection = 1; // represent GREEN
            } 
            else if (selection == GREEN_REMOTE_RED_SCORE || selection == RED_REMOTE_RED_SCORE) {
                print("*** Red player scored ***");
                selection = 2; // represent RED
            }
            context.gameObject->playerScore(selection);
        }
        else if (selection == GREEN_REMOTE_UNDO || selection == RED_REMOTE_UNDO) {
            print("*** Undo ***");
            context.gameObject->undo();
        }
        else {
            print("*** Invalid selection ***");
            showHelp();
        }

        std::this_thread::sleep_for(std::chrono::seconds(SCORE_DELAY));
        context.gameObject->loopGame();
    }

private:
    void showHelp() {
        print("------------");
        print("GREEN REMOTE:");
        print("   green remote green score: " + std::to_string(GREEN_REMOTE_GREEN_SCORE));
        print("   green remote red score:   " + std::to_string(GREEN_REMOTE_RED_SCORE));
        print("RED REMOTE:");
        print("   red remote green score:   " + std::to_string(RED_REMOTE_GREEN_SCORE));
        print("   red remote red score:     " + std::to_string(RED_REMOTE_RED_SCORE));
        print("------------");
    }
};

// ---------------------------------------------------------
// Helper: pick the correct State object for the given game action
// ---------------------------------------------------------
std::unique_ptr<IInputListenerState> createState(int currentAction) {
    switch (currentAction) {
    case PAIRING_MODE:
        return std::make_unique<PairingModeState>();
    case SLEEP_MODE:
        return std::make_unique<SleepModeState>();
    case REGULAR_PLAY_NO_SCORE:
        return std::make_unique<RegularGamePlayBeforeScoreState>();
    case REGULAR_PLAY_AFTER_SCORE:
    case AFTER_SLEEP_MODE:  // we can treat "AfterSleep" similar to "AfterScore"
        return std::make_unique<RegularGamePlayAfterScoreState>();
    default:
        // Fallback: do nothing. We could also return a "Null State" or
        // something safe. For demonstration, return the "AfterScore" logic.
        print("*** WARNING: Unknown state, defaulting to RegularGamePlayAfterScore ***");
        return std::make_unique<RegularGamePlayAfterScoreState>();
    }
}

/***************************************************************
 * run_remote_listener()
 *
 * This is the main loop that references the new State-based
 * design. Instead of a giant while-loop with many nested 
 * conditions, it delegates to the appropriate State object
 * each iteration.
 ***************************************************************/
void run_remote_listener(GameObject* gameObject, 
                         GameState* gameStatearg, 
                         Reset* reset) 
{
    // Setup some constants
    const int KEYBOARD_TIMEOUT = 120000;

    // Basic initialization
    GameState* gameState           = gameStatearg;
    RemoteLocker* remoteLocker     = new RemoteLocker(gameState);

    // We need the four input readers
    IInputWithTimer*    pairingInputWithTimer   = nullptr;
    IInputWithTimer*    noBlinkInputWithTimer   = nullptr;
    IInputWithTimer*    sleepingInputWithTimer  = nullptr;
    IGameInput*         gameInput               = nullptr;

    // This "no_score" variable was used in your original code
    bool no_score = true;

    // For setting up the scoreboard, blinkers, etc.
    Inputs* inputs                = new Inputs(gameObject->getPlayer1(),
                                               gameObject->getPlayer2(),
                                               gameObject->getPinInterface(),
                                               gameState);
    bool keyboard_off = true; // set to true to use the remotes

    // Some initial calls
    print("calling game object loop game...");
    gameObject->loopGame();
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Program to the scoreboard interface
    auto scoreboard = gameObject->getScoreBoard();
    scoreboard->setLittleDrawerFont("fonts/8x13B.bdf");
    std::signal(SIGINT, GameObject::_signalHandler);

    // Set up the screens/blinkers
    print("constructing blinkers...");
    RemotePairingScreen* remotePairingScreen = new RemotePairingScreen(scoreboard);
    PairingBlinker*      pairingBlinker      = new PairingBlinker(scoreboard);
    BlankBlinker*        blankBlinker        = new BlankBlinker();
    ScoreboardBlinker*   sleepingBlinker     = new ScoreboardBlinker(scoreboard);

    // Timeouts
    unsigned long pairing_timer  = 4000;
    unsigned long no_blink_timer = 4000;
    unsigned long sleeping_timer = 4000;

    bool is_on_pi = scoreboard->onRaspberryPi();
    print("is_on_pi: " + std::to_string(is_on_pi));

    // Decide whether to use keyboard-based or remote-based input
    if (is_on_pi && keyboard_off) {
        pairingInputWithTimer  = new RemoteInputWithTimer(pairingBlinker, inputs, pairing_timer);
        noBlinkInputWithTimer  = new RemoteInputWithTimer(blankBlinker,   inputs, no_blink_timer);
        sleepingInputWithTimer = new RemoteInputWithTimer(sleepingBlinker,inputs, sleeping_timer);
        gameInput              = new RemoteGameInput(inputs);
    } else {
        pairingInputWithTimer  = new KeyboardInputWithTimer(pairingBlinker, KEYBOARD_TIMEOUT);
        noBlinkInputWithTimer  = new KeyboardInputWithTimer(blankBlinker,   KEYBOARD_TIMEOUT);
        sleepingInputWithTimer = new KeyboardInputWithTimer(sleepingBlinker,KEYBOARD_TIMEOUT);
        gameInput              = new KeyboardGameInput();
    }

    // Build our context for the states
    InputListenerContext context(gameObject, 
                                  gameState, 
                                  reset, 
                                  remoteLocker,
                                  pairingInputWithTimer,
                                  noBlinkInputWithTimer,
                                  sleepingInputWithTimer,
                                  gameInput,
                                  remotePairingScreen,
                                  pairingBlinker,
                                  blankBlinker,
                                  sleepingBlinker,
                                  no_score);

    // Main loop: keep going while the game is running and not SIGINT
    while (gameState->gameRunning() && gSignalStatus != SIGINT) {
        // A small delay to mimic original code behavior
        std::this_thread::sleep_for(std::chrono::seconds(SCORE_DELAY));

        // Which state are we in?
        int currentAction = gameState->getCurrentAction();
        // If we haven't explicitly set an action yet, let's assume pairing:
        if (currentAction == 0) {
            currentAction = PAIRING_MODE;
            gameState->setCurrentAction(PAIRING_MODE);
        }

        // Create the appropriate state and handle input
        std::unique_ptr<IInputListenerState> state = createState(currentAction);
        state->handleInput(context);

        // (Optionally) gather set history - as the original code did
        // This is just to mimic the old code, no real logic done with it
        std::map<int, int> p1SetHistory = gameState->getPlayer1SetHistory();
        std::map<int, int> p2SetHistory = gameState->getPlayer2SetHistory();
        // We could do something with these ...
    }

    // Cleanup (some items are allocated inside this function)
    delete remoteLocker;
    delete pairingInputWithTimer;
    delete noBlinkInputWithTimer;
    delete sleepingInputWithTimer;
    delete gameInput;
    delete inputs;

    delete remotePairingScreen;
    delete pairingBlinker;
    delete blankBlinker;
    delete sleepingBlinker;

    print("Exiting run_remote_listener()...");
}
