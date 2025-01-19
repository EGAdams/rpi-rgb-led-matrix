# Persona
You are a world-class C++ Developer and seasoned user of the GoF Design Patterns
You always write modular, testable, and maintainable code.

# What we are building
We are building the begginning part of a tennis game.  We will eventually be adding this logic into a complete tennis game.  The reason why we are building this part first is because it is the most complex part of the game.  We want to get the logic right before we start adding anything else.

## Beginning Game States
Here are the states of the game that we want to focus on:
1. Pairing Mode
2. Sleep Mode
3. Regular Game Play Before any Scores
4. Regular Game Play After a Score

# Input types
* Remote Input - This is the input from the remote that is sent to the system.
## C++ code for the Remote Input to read the MCB23017 expander chip
```cpp
/**
 * @brief Retrieves user input, either from a remote control or keyboard, and handles the input with a timer.
 * 
 * This method is responsible for getting user input, either from a remote control or keyboard, and handling the input with a timer. 
 * If the input is from a remote control, it will continuously read the input from the MCP23017 expander chip until a valid selection is made.
 * If the input is from the keyboard, it will prompt the user to enter a selection.
 * The method also starts and stops a blinker during the input process, and tracks the time spent waiting for input.
 * 
 * @return int The selected input value.
 */
int InputWithTimer::getInput() {
    unsigned long sleep_start = GameTimer::gameMillis(); // Mark start time with game timer
    int selection;
    bool done = false;  
    print( "starting blinker from within InputWithTimer..." );
    _blinker->start();
    print( "getting input from within InputWithTimer..." );
    if ( REMOTE_INPUT == 1 ) {  
        while ( !done ) {                           // remote mode
            print( "*** reading selection from inputs... ***" );
            selection = _inputs->read_mcp23017_value();
            std::cout << "read selection from inputs: " << selection << std::endl;
            if ( selection == GREEN_REMOTE_GREEN_SCORE  || 
                 selection == GREEN_REMOTE_RED_SCORE    ||
                 selection == RED_REMOTE_GREEN_SCORE    ||
                 selection == RED_REMOTE_RED_SCORE ) {
                std::cout << "selection: " << selection << " triggered the done flag, exiting while loop..." << std::endl;
                done = true;
            } else { 
                // delay 250ms
                std::cout << "sleeping 250ms..." << std::endl; 
                GameTimer::gameDelay( 250 );
            }
        }
    } else if ( REMOTE_INPUT == 0 ) {               // Keyboard menu mode
        std::cout << "Enter your selection: ";
        std::cin >> selection;
        print( "made seleciton in InputWithTimer::getInput()... " );
        print( "selection: " << selection );

    } else {
        std::cout << "*** ERROR: Invalid input mode in InputWithTimer Object getInput() method. ***\n";
        exit( 1 );
    }
    _blinker->stop();
    unsigned long sleep_end = GameTimer::gameMillis(); // Mark end time with game timer
    _time_slept = sleep_end - sleep_start;
    return selection;
}
```

* Keyboard Input - This is the input from the keyboard.
## C++ Source Code for the Keyboard Input
```cpp
#ifndef KEYBOARD_INPUT_H
#define KEYBOARD_INPUT_H

#include "IInput.h"
#include <iostream>

class KeyboardInput : public IInput {
public:
    /**
     * @brief Gets user input from keyboard with validation and timeout handling
     * 
     * This method prompts for and reads keyboard input, validates it against 
     * allowed game commands (1-2 for player scores, 9 for undo), and handles
     * timeouts. It displays a prompt, reads stdin, and returns the validated
     * selection.
     * 
     * @return int The validated input selection (1-2 for scores, 9 for undo)
     * @throws std::runtime_error If input is invalid or timeout occurs
     */
    int getInput() override;

    /**
     * @brief Sets the timeout duration for keyboard input
     * 
     * @param milliseconds The timeout duration in milliseconds
     */
    void setTimeout(unsigned long milliseconds);

private:
    unsigned long _timeout_ms = 10000; // Default 10 second timeout
    bool validateInput(int selection);
};

#endif
```


# Beginning Logic Flow
When the power to the system is turned on, the system begins in in Pairing Mode.   At first, we want to set up the method to accept input from the keyboard so that we can test and debug it easier.  In a normal game, we would get remote input from an mcp23017 expander chip.  After we get it working in keyboard mode, we will swap out the neccessary code to use the MCP23017 expander chip as input instead of the keyboard.  So keep this in mind when you are designing the User Input Interface.


# Logic Flow Instructions

1. **Game Start**: The game begins in Pairing Mode, awaiting Green Player and Red Player scores.
2. **Sleep Mode Activation**:
   - If Pairing Mode lasts over 10 seconds, the system enters Sleep Mode and blinks a light to indicate this state.
3. **Exiting Sleep Mode**:
   - Any input (Green Player score, Red Player score, or Undo) exits Sleep Mode.
   - After exiting Sleep Mode, the system transitions to Regular Game Play (Before Scores).
4. **Timeout to Sleep**:
   - If Regular Game Play (Before Scores) is idle for 10 seconds, the system re-enters Sleep Mode.
5. **Scoring Transition**:
   - A score transitions the game to Regular Game Play, after which no timers are active.
6. **End of Match**:
   - After a Match win, the system enters Sleep Mode.
   - If a Green or Red Player score is pressed within 10 seconds of this Sleep Mode, the game resumes Regular Game Play, retaining game history to enable Undo.
7. **Undo After Match Win**:
   - Pressing Undo after exiting Sleep Mode takes the game back one point before the Match win.
8. **Cleared History**:
   - If Sleep Mode lasts over 10 seconds, any input transitions the system to Regular Game Play, but game history is cleared, and Undo cannot revert to the pre-Match state.

# My Thoughts
I think we may be able to use the Strategy Pattern to handle the different game states.  Since you are an expert in using Design Patterns, you should be able to think of other patterns that we could use. 

# C++ Source code for the Interfaces used in this system
```cpp
class IGameState {
public:
    virtual ~IGameState() = default;

    virtual bool gameRunning() const = 0;                      // Indicates if the game is still running
    virtual void setCurrentAction(EnumGameState action) = 0;   // Sets the current "action state"
    virtual EnumGameState getCurrentAction() const = 0;        // Gets the current "action state"

    // For demonstration, we assume the real IGameState has ways to access set history
    virtual std::map<int, int> getPlayer1SetHistory() const = 0;
    virtual std::map<int, int> getPlayer2SetHistory() const = 0;

    // Additional stubs to track whether any score has been made or if the match is complete
    virtual bool anyScoreMadeYet() const = 0;      // returns true if at least one point has been scored
    virtual bool matchComplete() const = 0;        // returns true if the match is completely won
};

class IGameObject {
public:
    virtual ~IGameObject() = default;

    virtual void loopGame() = 0;               // Some continuous loop function
    virtual void resetMatch() = 0;             // Resets match
    virtual void playerScore(int player) = 0;  // Increments a player's score
    virtual void undo() = 0;                   // Undo the last scoring event

    // Our scoreboard and history
    virtual IScoreBoard* getScoreBoard() = 0;
    virtual IGameState* getGameState() = 0;

    // Access to game history container
    class IHistory {
    public:
        virtual ~IHistory() = default;
        virtual void clearHistory() = 0;
    };
    virtual IHistory* getHistory() = 0;

    // For signal handling
    static void _signalHandler(int signal) { gSignalStatus = signal; }
};

class IRemoteLocker {
public:
    virtual ~IRemoteLocker() = default;
    // For demonstration: returns non-zero if "player not serving"
    virtual int playerNotServing(int selection) = 0;
};

class IInputs {
public:
    virtual ~IInputs() = default;

    // For a remote scenario, you read some hardware register
    virtual int read_mcp23017_value() = 0;
};

class IScoreBoard {
public:
    virtual ~IScoreBoard() = default;

    virtual bool onRaspberryPi() const = 0;
    virtual void setLittleDrawerFont(const std::string& fontPath) = 0;
    virtual void clearScreen() = 0;
    virtual void update() = 0;
};

class IPairingBlinker {
public:
    virtual ~IPairingBlinker() = default;

    virtual void stop() = 0;
    virtual bool awake() const = 0;

    virtual void setGreenPlayerPaired(bool paired) = 0;
    virtual void setRedPlayerPaired(bool paired) = 0;
};

class IScoreboardBlinker {
public:
    virtual ~IScoreboardBlinker() = default;
};

class IRemotePairingScreen {
public:
    virtual ~IRemotePairingScreen() = default;

    virtual bool inPairingMode() const = 0;
    virtual void greenPlayerPressed() = 0;
    virtual void redPlayerPressed() = 0;
};

class IInputWithTimer {
public:
    virtual ~IInputWithTimer() = default;
    virtual int getInput() = 0;
    virtual long getTimeSlept() const = 0; // returns how long we slept/waited in ms
};
```

# C++ Source Code for the Tennis Game
```cpp
void run_remote_listener(IGameObject* gameObject, IGameState* gameState, IReset* /*reset*/, IInputs* inputs) {
    print("entered run remote listener method...");
    int loop_count = 0;
    int selection = 0;
    print("calling game object loop game...");
    gameObject->loopGame();
    std::this_thread::sleep_for(std::chrono::seconds(1));

    auto scoreboard = gameObject->getScoreBoard(); // Program to the IScoreBoard interface
    scoreboard->setLittleDrawerFont("fonts/8x13B.bdf");

    std::signal(SIGINT, IGameObject::_signalHandler);

    print("constructing remote pairing screen..."); // Program to the IRemotePairingScreen interface
    IRemotePairingScreen* remotePairingScreen = new RemotePairingScreen(scoreboard);

    print("constructing pairing blinker...");
    IPairingBlinker* pairingBlinker = new PairingBlinker(scoreboard);

    print("constructing input with timer...");
    IInputWithTimer* inputWithTimer = new InputWithTimer(pairingBlinker, inputs);
    print("finished constructing input with timer...");

    bool is_on_pi = scoreboard->onRaspberryPi();
    print("is_on_pi: " + std::to_string(is_on_pi));

    while (gameState->gameRunning() && gSignalStatus != SIGINT) { /*/// Begin Game Loop ///*/
        std::this_thread::sleep_for(std::chrono::seconds(SCORE_DELAY));

        // if remote pairing, write the words. if not, snap out of the loop
        while (remotePairingScreen->inPairingMode() && pairingBlinker->awake()) {
            print("inside remote pairing screen. before starting input timer...");

            if (REMOTE_INPUT == 1) {
                print("calling inputWithTimer->getInput()...");
                selection = inputWithTimer->getInput();
            } else {
                print("Inside pairing mode. enter 1 for Green pair and 2 for Red pair");
                std::cin >> selection;
                print("*** inside remote listener getting remote selection ***");
                print("selection: " + std::to_string(selection));
            }

            if (selection == GREEN_REMOTE_GREEN_SCORE) {
                remotePairingScreen->greenPlayerPressed();
                pairingBlinker->setGreenPlayerPaired(true);
            } else if (selection == RED_REMOTE_RED_SCORE) {
                remotePairingScreen->redPlayerPressed();
                pairingBlinker->setRedPlayerPaired(true);
            } else {
                std::cout << "*** Invalid selection during remote pairing. ***\n";
                GameTimer::gameDelay(1000);
            }
        }

        if (!pairingBlinker->awake()) {
            print("pairing blinker is not awake, stopping it... ");
            pairingBlinker->stop();
            print("pairing blinker stopped. now putting in sleep mode...");
            gameState->setCurrentAction(SLEEP_MODE);
        }

        if (gameState->getCurrentAction() == SLEEP_MODE) {
            print("current action is SLEEP_MODE");
            IScoreboardBlinker* blinker = new ScoreboardBlinker(scoreboard);

            IInputWithTimer* inputWithTimer2 = new InputWithTimer(nullptr, inputs);
            int selection = inputWithTimer2->getInput();

            gameState->setCurrentAction(AFTER_SLEEP_MODE);
            std::cout << "time slept: " << inputWithTimer2->getTimeSlept() << std::endl;

            if (selection == GREEN_REMOTE_GREEN_SCORE ||
                selection == GREEN_REMOTE_RED_SCORE ||
                (inputWithTimer2->getTimeSlept() > MAX_SLEEP * 1000)) {
                print("reset match.");
                gameObject->resetMatch();
                print("done resetting match.");
                if (inputWithTimer2->getTimeSlept() > MAX_SLEEP * 1000) {
                    print("time slept: " + std::to_string(inputWithTimer2->getTimeSlept() / 1000) + " seconds.");
                    print("clearing History because max sleep time has been reached or exceeded.");
                    gameObject->getHistory()->clearHistory();
                    print("done clearing history because max sleep time has been reached or exceeded.");
                }
                delete inputWithTimer2;
                continue;
            }

            print("setting game state current action to after sleep mode");
            gameState->setCurrentAction(AFTER_SLEEP_MODE);
            print("*** Going into last Match! ***");
            print("clearing screen...");
            scoreboard->clearScreen();
            print("cleared screen.");
            scoreboard->update();
            print("updated scoreboard.");

            delete blinker;
            delete inputWithTimer2;
        } else {
            if (REMOTE_INPUT == 0) {
                std::cin >> selection;
                print("selection: " + std::to_string(selection));
            } else {
                bool done = false;
                while (!done) {
                    selection = inputs->read_mcp23017_value();
                    std::cout << "read selection from inputs: " << selection << std::endl;
                    if (selection == GREEN_REMOTE_GREEN_SCORE ||
                        selection == GREEN_REMOTE_RED_SCORE ||
                        selection == RED_REMOTE_GREEN_SCORE ||
                        selection == RED_REMOTE_RED_SCORE ||
                        selection == GREEN_REMOTE_UNDO ||
                        selection == RED_REMOTE_UNDO) {
                        std::cout << "selection: " << selection << " triggered the done flag, exiting while loop..." << std::endl;
                        done = true;
                    } else {
                        std::cout << "sleeping 250ms..." << std::endl;
                        GameTimer::gameDelay(250);
                    }
                }
            }
        }

        int serve_flag = gameObject->playerNotServing(selection);
        print("*** serve_flag: " + std::to_string(serve_flag) + " ***");
        if (serve_flag) {
            print("*** Warning: player not serving! ***");
            continue;
        }

        print("setting player button to selection: " + std::to_string(selection) + " before calling loopGame()...");

        if (selection == GREEN_REMOTE_GREEN_SCORE ||
            selection == GREEN_REMOTE_RED_SCORE ||
            selection == RED_REMOTE_GREEN_SCORE ||
            selection == RED_REMOTE_RED_SCORE) {
            if (selection == GREEN_REMOTE_GREEN_SCORE || selection == RED_REMOTE_GREEN_SCORE) {
                print("*** \n\n\nGreen player scored ***\n\n\n");
                selection = 1; // represent GREEN
            } else if (selection == GREEN_REMOTE_RED_SCORE || selection == RED_REMOTE_RED_SCORE) {
                print("\n\n\n*** Red player scored ***\n\n\n");
                selection = 2; // represent RED
            }
            gameObject->playerScore(selection);
        } else if (selection == GREEN_REMOTE_UNDO || selection == RED_REMOTE_UNDO) {
            print("\n\n\n*** Undo ***\n\n\n");
            gameObject->undo();
        } else {
            std::cout << "\n\n\n*** Invalid selection ***\n\n\n" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(SCORE_DELAY));
            continue;
        }

        std::this_thread::sleep_for(std::chrono::seconds(SCORE_DELAY));
        gameObject->loopGame();  // handle the player score flag
        loop_count++;

        std::map<int, int> _player1_set_history = gameState->getPlayer1SetHistory();
        std::map<int, int> _player2_set_history = gameState->getPlayer2SetHistory();
    } ///////// End Game Loop /////////

    delete remotePairingScreen;
    delete pairingBlinker;
    delete inputWithTimer;
}
```

# Your Task
Rewrite the run_remote_listener to conform to the specifications.  If you need more Interfaces, just put them in your code block answer.  Write all of the C++ code into on code block.  Don't worry about rewriting the Interfaces that we already have, just rewrite the run_remote_listener method and the main method.  I want to be able to take the C++ code block that you write and compile it and run it.  For the main function, inject the keyboard type input, but keep in mind that I will need to swap it out seamlessly with the remote input when we decide to move this system to production.

jan 18 2025, 8:53pm  tyrone panera saturday
https://chatgpt.com/share/678c5af3-82fc-8006-a533-0f8678cc5772
