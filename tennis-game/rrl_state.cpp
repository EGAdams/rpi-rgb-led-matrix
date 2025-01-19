/********************************************************************************
 * Example of a Refactored run_remote_listener Method Using a Simple 
 * State-Machine (Strategy-like) Approach
 *
 * This code shows how you might reorganize the existing run_remote_listener
 * function to better reflect the game flow requirements and separate concerns. 
 * 
 * For simplicity, the code below places all classes/interfaces in one code 
 * block. In a real-world scenario, you would want to split these out into 
 * separate header and source files. 
 *
 * NOTE: This is illustrative code. You may need to adapt it to your 
 *       existing class designs, method signatures, and constructor 
 *       requirements.
 ********************************************************************************/

#include <iostream>
#include <memory>
#include <map>
// #include <signal.h>
#include <csignal> // for signal handling
#include <thread>
#include <chrono>
#include <functional>
#include <ctime>

// -------------------- Mock/Placeholder Interfaces & Constants -------------------- //
static volatile sig_atomic_t gSignalStatus = 0;
#define SIGINT 2

// Keypress values you might receive from either the remote or keyboard:
static const int GREEN_REMOTE_GREEN_SCORE = 1;
static const int GREEN_REMOTE_RED_SCORE   = 2;
static const int RED_REMOTE_GREEN_SCORE   = 3;
static const int RED_REMOTE_RED_SCORE     = 4;
static const int GREEN_REMOTE_UNDO        = 5;
static const int RED_REMOTE_UNDO          = 6;

// You may set this to 0 if you want keyboard input, 1 if you want remote input
static const int REMOTE_INPUT = 0;

// Sleep time thresholds, in seconds
static const int PAIRING_TIMEOUT = 10;     // 10 seconds for pairing
static const int SLEEP_TIMEOUT   = 10;     // 10 seconds in sleep
static const int NO_SCORE_TIMEOUT = 10;    // 10 seconds in "RegularGameBeforeScores"

// For convenience in the example
static const int SCORE_DELAY = 1;
static const int MAX_SLEEP   = 10; // in seconds, for old snippet usage

// Forward Declarations
class IGameObject;
class IGameState;
class IRemoteLocker;
class IInputs;
class IScoreBoard;
class IPairingBlinker;
class IScoreboardBlinker;
class IRemotePairingScreen;
class IInputWithTimer;

// Action states for the tennis game
enum EnumGameState {
    PAIRING_MODE = 0,
    SLEEP_MODE,
    REGULAR_GAME_BEFORE_SCORES,
    REGULAR_GAME_AFTER_SCORE
};

// -------------------- Mocked Interfaces (stubs) -------------------- //
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


class RemoteLocker : public IRemoteLocker {
    std::shared_ptr<IGameState> m_gameState;
public:
    RemoteLocker(std::shared_ptr<IGameState> gs) : m_gameState(gs) {}

    int playerNotServing(int /*selection*/) override {
        // For demonstration, always return 0 (meaning "no problem").
        // In real code, you'd compare the selection to the server.
        return 0;
    }
};

// Just a naive pair of blinkers for demonstration
class PairingBlinker : public IPairingBlinker {
    std::shared_ptr<IScoreBoard> m_scoreboard;
    bool m_awake{true};
    bool m_greenPaired{false};
    bool m_redPaired{false};

public:
    PairingBlinker(std::shared_ptr<IScoreBoard> sb) : m_scoreboard(sb) {}

    void stop() override { m_awake = false; }
    bool awake() const override { return m_awake; }

    void setGreenPlayerPaired(bool paired) override { m_greenPaired = paired; }
    void setRedPlayerPaired(bool paired) override { m_redPaired = paired; }
};

class ScoreboardBlinker : public IScoreboardBlinker {
    // You'd do your blinking logic here
public:
    ScoreboardBlinker(std::shared_ptr<IScoreBoard> /*sb*/) {}
};

class RemotePairingScreen : public IRemotePairingScreen {
    std::shared_ptr<IScoreBoard> m_scoreboard;
    bool m_pairingMode{true};

public:
    RemotePairingScreen(std::shared_ptr<IScoreBoard> sb) : m_scoreboard(sb) {}

    bool inPairingMode() const override { return m_pairingMode; }
    void greenPlayerPressed() override {
        // in real code, you'd show some UI feedback
        // if Red is also pressed, you might exit pairing mode
    }
    void redPlayerPressed() override {
        // in real code, you'd show some UI feedback
        // if Green is also pressed, you might exit pairing mode
    }
    // For demonstration, let the outside logic turn inPairingMode off
    void setPairingComplete() { m_pairingMode = false; }
};

class InputWithTimer : public IInputWithTimer {
    std::shared_ptr<IPairingBlinker> m_pairingBlinker;
    std::shared_ptr<IInputs> m_inputs;
    long m_timeSleptMs{0};

public:
    InputWithTimer(std::shared_ptr<IPairingBlinker> pairingBlinker, std::shared_ptr<IInputs> inputs)
        : m_pairingBlinker(pairingBlinker), m_inputs(inputs) {}

    int getInput() override {
        // For demonstration, this reads from console if REMOTE_INPUT == 0
        // or from m_inputs->read_mcp23017_value() if REMOTE_INPUT == 1

        using namespace std::chrono;
        auto start = steady_clock::now();
        int selection = -1;

        if (REMOTE_INPUT == 1) {
            // Attempt a "blocking" read from hardware or something similar
            selection = m_inputs->read_mcp23017_value();
            auto end   = steady_clock::now();
            m_timeSleptMs =
                duration_cast<milliseconds>(end - start).count();
        }
        else {
            // Attempt a "blocking" read from stdin
            auto endWaitTime = steady_clock::now() + seconds(PAIRING_TIMEOUT);
            while (true) {
                if (std::cin.rdbuf()->in_avail() > 0) {
                    std::cin >> selection;
                    break;
                }
                if (steady_clock::now() > endWaitTime) {
                    // timed out
                    selection = -1;
                    break;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
            auto end = steady_clock::now();
            m_timeSleptMs = duration_cast<milliseconds>(end - start).count();
        }
        return selection;
    }

    long getTimeSlept() const override {
        return m_timeSleptMs;
    }
};

// A simple game timer utility for demonstration
struct GameTimer {
    static void gameDelay(int ms) {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }
};

// A convenience function for printing
static void print(const std::string& msg) {
    std::cout << "[INFO] " << msg << std::endl;
}

// -------------------- Refactored run_remote_listener -------------------- //
void run_remote_listener(
    IGameObject* gameObject,
    IGameState* gameState,
    /* Possibly an IReset* reset, omitted for brevity */ 
    IInputs* inputs
) {
    std::shared_ptr<IGameState> spGameState(
        dynamic_cast<IGameState*>( gameState ) ? gameState : nullptr
    );

    // Create the RemoteLocker
    std::unique_ptr<IRemoteLocker> remoteLocker = std::make_unique<RemoteLocker>(spGameState);

    print("entered run_remote_listener method...");

    // Start the loopGame routine in a separate thread or directly
    // We'll just call it once here for illustration
    gameObject->loopGame();
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Setup the scoreboard
    auto scoreboard = gameObject->getScoreBoard();
    scoreboard->setLittleDrawerFont("fonts/8x13B.bdf");

    std::signal(SIGINT, IGameObject::_signalHandler);

    // Create the remote pairing screen
    print("constructing remote pairing screen...");
    std::unique_ptr<IRemotePairingScreen> remotePairingScreen =
        std::make_unique<RemotePairingScreen>(scoreboard);

    // Create the pairing blinker
    print("constructing pairing blinker...");
    std::shared_ptr<IPairingBlinker> pairingBlinker =
        std::make_shared<PairingBlinker>(scoreboard);

    // Create input with timer
    print("constructing input with timer...");
    std::unique_ptr<IInputWithTimer> inputWithTimer =
        std::make_unique<InputWithTimer>(
            pairingBlinker, 
            std::shared_ptr<IInputs>(inputs, [](IInputs*){})
        );

    print("finished constructing input with timer...");

    bool is_on_pi = scoreboard->onRaspberryPi();
    print("is_on_pi: " + std::to_string(is_on_pi));

    // Initialize some local state for pairing
    bool greenPaired = false;
    bool redPaired   = false;

    // The game always starts in Pairing Mode
    spGameState->setCurrentAction(PAIRING_MODE);

    // For measuring "time in state" so we know when to go to Sleep
    auto stateEntryTime = std::chrono::steady_clock::now();

    // Helper lambda for resetting the "time in state"
    auto resetStateTimer = [&]() {
        stateEntryTime = std::chrono::steady_clock::now();
    };

    // Helper lambda to check if we exceeded some timeout in seconds
    auto exceededTimeout = [&](int seconds) {
        auto now   = std::chrono::steady_clock::now();
        auto diff  = std::chrono::duration_cast<std::chrono::seconds>(now - stateEntryTime).count();
        return (diff >= seconds);
    };

    // Mark that we have not cleared history since the last match ended
    // (Used to track "if we woke up within 10 seconds or not")
    bool matchEndedAndWentToSleep = false;
    auto sleepEntryTime = std::chrono::steady_clock::now();

    while (spGameState->gameRunning() && gSignalStatus != SIGINT) {

        EnumGameState currentAction = spGameState->getCurrentAction();

        switch (currentAction) 
        {
        case PAIRING_MODE:
        {
            // The game is in Pairing Mode. Wait for green then red. 
            // If >10s pass with no progress, we go to SLEEP_MODE

            print("=== PAIRING_MODE ===");
            // We'll remain in a loop until both players are paired or we timeout
            while (!greenPaired || !redPaired) 
            {
                if (exceededTimeout(PAIRING_TIMEOUT)) {
                    // Timeout -> go to sleep
                    print("Pairing timeout occurred, going to Sleep Mode.");
                    spGameState->setCurrentAction(SLEEP_MODE);
                    // We'll do the sleep logic in the main loop
                    break;
                }

                // Attempt to read an input
                int selection = -1;
                if (REMOTE_INPUT == 1) {
                    print("calling inputWithTimer->getInput()...");
                    selection = inputWithTimer->getInput();
                }
                else {
                    // If REMOTE_INPUT == 0, prompt for keyboard input
                    print("Inside pairing mode. Enter 1 for Green pair, 2 for Red pair:");
                    selection = inputWithTimer->getInput(); // same object, but it'll read cin
                    print("Selection: " + std::to_string(selection));
                }

                if (selection == GREEN_REMOTE_GREEN_SCORE) {
                    print("Green remote paired...");
                    greenPaired = true;
                    remotePairingScreen->greenPlayerPressed();
                    pairingBlinker->setGreenPlayerPaired(true);
                }
                else if (selection == RED_REMOTE_RED_SCORE) {
                    print("Red remote paired...");
                    redPaired = true;
                    remotePairingScreen->redPlayerPressed();
                    pairingBlinker->setRedPlayerPaired(true);
                }
                else if (selection == -1) {
                    // This indicates a timeout waiting for user input
                    // We'll jump out and let the while loop handle going to SLEEP_MODE
                    continue;
                }
                else if (selection != -1) {
                    std::cout << "*** Invalid selection during remote pairing. ***\n";
                    GameTimer::gameDelay(1000);
                }

                // If both are paired, we can exit pairing
                if (greenPaired && redPaired) {
                    print("Both remotes are now paired. Exiting Pairing Mode...");
                    // We'll break the while, set the next state
                    break;
                }
            }

            // If we are still in PairingMode but greenPaired && redPaired, 
            // we move to the next state
            if (greenPaired && redPaired && spGameState->getCurrentAction() == PAIRING_MODE) {
                // Stop the pairing blinker
                pairingBlinker->stop();
                print("Pairing blinker stopped. Transitioning to REGULAR_GAME_BEFORE_SCORES...");
                spGameState->setCurrentAction(REGULAR_GAME_BEFORE_SCORES);
            }
            resetStateTimer();
            break;
        }
        case SLEEP_MODE:
        {
            print("=== SLEEP_MODE ===");
            // Scoreboard should blink
            std::unique_ptr<IScoreboardBlinker> blinker =
                std::make_unique<ScoreboardBlinker>(scoreboard);

            // In Sleep Mode, an input of Green Score, Red Score, or Undo will wake the system.
            // If the system has been in Sleep Mode for over 10s, and a button is pressed, 
            // we clear game history.

            // We'll block for input here
            std::unique_ptr<IInputWithTimer> inputWithTimer2 =
                std::make_unique<InputWithTimer>(
                    std::shared_ptr<IPairingBlinker>(), // no pairing blinker
                    std::shared_ptr<IInputs>(inputs, [](IInputs*){})
                );

            print("Waiting for input to break out of Sleep Mode...");

            // We'll do a blocking read for a single input:
            int selection = -1;
            selection = inputWithTimer2->getInput();
            long sleptMillis = inputWithTimer2->getTimeSlept();
            long sleptSecs   = sleptMillis / 1000;

            print("Slept for: " + std::to_string(sleptSecs) + " seconds before input arrived.");

            // ANY valid button press should break from Sleep. But let's see if 
            // it's a "score" or "undo"
            bool isScoreOrUndo =
                (selection == GREEN_REMOTE_GREEN_SCORE ||
                 selection == GREEN_REMOTE_RED_SCORE   ||
                 selection == RED_REMOTE_GREEN_SCORE   ||
                 selection == RED_REMOTE_RED_SCORE     ||
                 selection == GREEN_REMOTE_UNDO        ||
                 selection == RED_REMOTE_UNDO);

            // If no valid input, we'll just keep waiting
            if (!isScoreOrUndo) {
                print("Invalid or no input in Sleep Mode, continuing...");
                // If we want to keep waiting: do not change state
                // However, we should check if we want to break to let the while() 
                // re-run. We'll just continue
                continue;
            }

            // We have a valid input. We must see how long we were asleep:
            if (sleptSecs > SLEEP_TIMEOUT) {
                // Clear the match and history
                print("Over " + std::to_string(SLEEP_TIMEOUT) + "s in sleep. Clearing match & history...");
                gameObject->resetMatch();
                gameObject->getHistory()->clearHistory();
                print("Done clearing match & history.");
            }
            else {
                // If the match was complete, but we woke within 10s, 
                // we keep match history. So do nothing extra. 
                // The user might press "Undo" to revert the last point 
                // after a match was won, etc.
                print("Woke from Sleep within " + std::to_string(SLEEP_TIMEOUT) + "s. Retaining match history...");
            }

            // Decide if the next state is "REGULAR_GAME_BEFORE_SCORES" or "REGULAR_GAME_AFTER_SCORE"
            // If no points have been scored so far, go to BEFORE_SCORES, 
            // else AFTER_SCORE.
            if (spGameState->anyScoreMadeYet()) {
                spGameState->setCurrentAction(REGULAR_GAME_AFTER_SCORE);
            } else {
                spGameState->setCurrentAction(REGULAR_GAME_BEFORE_SCORES);
            }
            resetStateTimer();
            // Clear the scoreboard for a fresh display (if desired)
            scoreboard->clearScreen();
            scoreboard->update();
            break;
        }
        case REGULAR_GAME_BEFORE_SCORES:
        {
            print("=== REGULAR_GAME_BEFORE_SCORES ===");
            // If no input for >10s, go to sleep.
            // If a score is made, we move to AFTER_SCORE

            // We'll wait for input up to some small chunk of time, 
            // repeatedly checking if we timed out

            int selection = -1;

            // Check if we timed out
            if (exceededTimeout(NO_SCORE_TIMEOUT)) {
                print("No scores after " + std::to_string(NO_SCORE_TIMEOUT) + "s, going to Sleep Mode...");
                spGameState->setCurrentAction(SLEEP_MODE);
                resetStateTimer();
                break;
            }

            // Attempt to read an input
            if (REMOTE_INPUT == 0) {
                // Keyboard approach
                if (std::cin.rdbuf()->in_avail() > 0) {
                    std::cin >> selection;
                } else {
                    // no input yet, let's wait a short time
                    std::this_thread::sleep_for(std::chrono::milliseconds(200));
                    break;  // let the while re-check
                }
            } else {
                // Remote approach
                selection = inputs->read_mcp23017_value();
                if (selection < 0) {
                    // invalid read, keep waiting
                    std::this_thread::sleep_for(std::chrono::milliseconds(200));
                    break;
                }
            }
            if (selection != -1) {
                print("Selection: " + std::to_string(selection));
            }

            // Validate if it's a recognized input
            if (selection == GREEN_REMOTE_GREEN_SCORE ||
                selection == GREEN_REMOTE_RED_SCORE   ||
                selection == RED_REMOTE_GREEN_SCORE   ||
                selection == RED_REMOTE_RED_SCORE ) 
            {
                // Score
                // Check if they're serving:
                int serve_flag = remoteLocker->playerNotServing(selection);
                if (serve_flag) {
                    print("*** Warning: player not serving! ***");
                    break; 
                }
                // Convert to internal player ID
                int player = (selection == GREEN_REMOTE_GREEN_SCORE || 
                              selection == RED_REMOTE_GREEN_SCORE) ? 1 : 2;
                gameObject->playerScore(player);

                // Now that a score has been made, transition to AFTER_SCORE
                spGameState->setCurrentAction(REGULAR_GAME_AFTER_SCORE);
                resetStateTimer();
            }
            else if (selection == GREEN_REMOTE_UNDO || selection == RED_REMOTE_UNDO) {
                // If there's no score to undo, typically do nothing or beep
                print("*** Undo Pressed ***");
                gameObject->undo();
            }
            else if (selection == -1) {
                // Nothing read
            }
            else {
                // invalid input
                print("*** Invalid selection ***");
            }
            gameObject->loopGame();

            // If the match is complete unexpectedly, go to sleep
            if (spGameState->matchComplete()) {
                print("Match completed. Going to Sleep Mode...");
                spGameState->setCurrentAction(SLEEP_MODE);
                matchEndedAndWentToSleep = true;
                sleepEntryTime = std::chrono::steady_clock::now();
                resetStateTimer();
            }
            break;
        }
        case REGULAR_GAME_AFTER_SCORE:
        {
            // There's no timer. We keep reading input. 
            // If final match is won, go to Sleep Mode.
            print("=== REGULAR_GAME_AFTER_SCORE ===");

            int selection = -1;
            if (REMOTE_INPUT == 0) {
                if (std::cin.rdbuf()->in_avail() > 0) {
                    std::cin >> selection;
                } else {
                    std::this_thread::sleep_for(std::chrono::milliseconds(200));
                    break;
                }
            } else {
                selection = inputs->read_mcp23017_value();
                if (selection < 0) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(200));
                    break;
                }
            }
            print("Selection: " + std::to_string(selection));

            // Validate if it's recognized
            if (selection == GREEN_REMOTE_GREEN_SCORE ||
                selection == GREEN_REMOTE_RED_SCORE   ||
                selection == RED_REMOTE_GREEN_SCORE   ||
                selection == RED_REMOTE_RED_SCORE ) 
            {
                int serve_flag = remoteLocker->playerNotServing(selection);
                if (serve_flag) {
                    print("*** Warning: player not serving! ***");
                    break;
                }
                int player = (selection == GREEN_REMOTE_GREEN_SCORE || 
                              selection == RED_REMOTE_GREEN_SCORE) ? 1 : 2;
                gameObject->playerScore(player);
            }
            else if (selection == GREEN_REMOTE_UNDO || selection == RED_REMOTE_UNDO) {
                print("*** Undo ***");
                gameObject->undo();
            }
            else {
                // invalid input
                print("*** Invalid selection ***");
                std::this_thread::sleep_for(std::chrono::seconds(SCORE_DELAY));
                break;
            }

            // Let the game process the scoring
            gameObject->loopGame();

            // If the match is now complete, go to Sleep
            if (spGameState->matchComplete()) {
                print("Match completed. Going to Sleep Mode...");
                spGameState->setCurrentAction(SLEEP_MODE);
                matchEndedAndWentToSleep = true;
                sleepEntryTime = std::chrono::steady_clock::now();
                resetStateTimer();
            }
            break;
        }
        } // end switch

        // End of main "state" switch. The while loop then continues,
        // re-checking currentAction each iteration.
    } // end while(gameRunning...)

    print("Exiting run_remote_listener...");
}

/************************************************************************************
 * In real usage, you'd implement the rest of your system (IGameObject, IGameState, 
 * scoreboard classes, etc.) in separate .cpp/.h files. This single code block is 
 * just to illustrate how one might unify all logic for demonstration.
 ************************************************************************************/

// Include the interfaces and run_remote_listener implementation here
// Or include the appropriate header file

// Mock implementations for interfaces (just enough to run)
class MockGameState : public IGameState {
    EnumGameState currentAction = PAIRING_MODE;
    bool running = true;

public:
    bool gameRunning() const override { return running; }
    void setCurrentAction(EnumGameState action) override { currentAction = action; }
    EnumGameState getCurrentAction() const override { return currentAction; }
    std::map<int, int> getPlayer1SetHistory() const override { return {}; }
    std::map<int, int> getPlayer2SetHistory() const override { return {}; }
    bool anyScoreMadeYet() const override { return false; }
    bool matchComplete() const override { return false; }
};

class MockHistory : public IGameObject::IHistory {
public:
    void clearHistory() override { std::cout << "Clearing game history...\n"; }
};

class MockScoreBoard : public IScoreBoard {
public:
    bool onRaspberryPi() const override { return false; }
    void setLittleDrawerFont(const std::string& fontPath) override {
        std::cout << "Setting font: " << fontPath << "\n";
    }
    void clearScreen() override { std::cout << "Clearing screen...\n"; }
    void update() override { std::cout << "Updating scoreboard...\n"; }
};

class MockGameObject : public IGameObject {
    std::shared_ptr<IScoreBoard> scoreboard;

public:
    MockGameObject() {
        scoreboard = std::make_shared<MockScoreBoard>();
    }

    void loopGame() override { std::cout << "Game looping...\n"; }
    void resetMatch() override { std::cout << "Match reset.\n"; }
    void playerScore(int player) override { std::cout << "Player " << player << " scored.\n"; }
    void undo() override { std::cout << "Undo last action.\n"; }
    std::shared_ptr<IScoreBoard> getScoreBoard() override { return scoreboard; }
    std::shared_ptr<IGameState> getGameState() override { return std::make_shared<MockGameState>(); }
    std::shared_ptr<IHistory> getHistory() override {
        return std::make_shared<MockHistory>();
    }
};


class MockInputs : public IInputs {
public:
    int read_mcp23017_value() override {
        // static int count = 0;
        // return ++count % 5 + 1; // Cycle through mock inputs
        int input = std::cin.get();
        return input;
    }
};


// Main function
int main() {
    auto gameState = std::make_shared<MockGameState>(); // Create mock objects
    auto gameObject = std::make_shared<MockGameObject>();
    auto inputs = std::make_shared<MockInputs>();
    run_remote_listener(gameObject.get(), gameState.get(), inputs.get()); // Run the listener
    return 0;
}
