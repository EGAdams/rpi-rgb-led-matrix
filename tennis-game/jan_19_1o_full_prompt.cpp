/**************************************************************************************
 * Example C++ implementation illustrating how one might rewrite the run_remote_listener 
 * function and a corresponding main() to demonstrate the requested design logic.
 * 
 * IMPORTANT NOTES:
 * 1) In real production code, you'll reuse your existing Interfaces (IGameObject, IGameState, etc.) 
 *    and wire them into your real classes. Here, minimal "stub" implementations are provided so that 
 *    this example compiles and runs as-is. 
 * 2) The code below uses a simplified "state machine" approach that manages the requested game states:
 *    - Pairing Mode
 *    - Sleep Mode
 *    - Regular Game Play Before any Scores
 *    - Regular Game Play After a Score
 * 3) A Keyboard-based IInputs implementation is used here for demonstration. 
 *    You can swap in your MCP23017-based remote input simply by injecting your real remote-based 
 *    IInputs implementation in main() in place of the Keyboard one.
 * 4) This example focuses on the logic flow and timeouts. Many details (like actual scoreboard 
 *    rendering, pairing blinkers, scoreboard blinkers, etc.) are minimal stubs, but you can integrate 
 *    your real classes in place of these stubs for a production build.
 *
 * Compile and run this file as a standalone demo. 
 **************************************************************************************/

#include <iostream>
#include <map>
#include <csignal>
#include <chrono>
#include <thread>
#include <atomic>
#include <memory>
#include <stdexcept>
#include <string>

/*******************************************
 * Forward declarations for the real interfaces
 * (Here, we only provide minimal placeholders
 * so this example compiles and runs.)
 *******************************************/
class IInputs;
class IGameState;
class IGameObject;
class IScoreBoard;
class IRemotePairingScreen;
class IPairingBlinker;
class IScoreboardBlinker;
class IInputWithTimer;

// Example enumerations to demonstrate scoring constants
static const int GREEN_REMOTE_GREEN_SCORE = 1;
static const int GREEN_REMOTE_RED_SCORE   = 2;
static const int RED_REMOTE_GREEN_SCORE   = 3;
static const int RED_REMOTE_RED_SCORE     = 4;
static const int GREEN_REMOTE_UNDO        = 9;
static const int RED_REMOTE_UNDO          = 9; 
//  ^ For a simple demo, we'll treat GREEN_REMOTE_UNDO and RED_REMOTE_UNDO both as 9

// Mock REMOTE_INPUT toggle.  0 = Keyboard, 1 = Remote
static const int REMOTE_INPUT             = 0; 

// Some Sleep time constants
static const unsigned long MAX_SLEEP      = 10; // e.g. 10 seconds
static const unsigned long SCORE_DELAY    = 1;  // short delay after each score for demonstration

// Pretend global signal status for SIGINT
std::atomic<int> gSignalStatus{0};

/*******************************************
 * Minimal mock definitions for real interfaces
 *******************************************/
class IGameState {
public:
    virtual ~IGameState() = default;

    virtual bool gameRunning() const = 0;
    virtual void setCurrentAction(int action) = 0;
    virtual int  getCurrentAction() const = 0;

    virtual std::map<int, int> getPlayer1SetHistory() const = 0;
    virtual std::map<int, int> getPlayer2SetHistory() const = 0;

    virtual bool anyScoreMadeYet() const = 0;
    virtual bool matchComplete() const = 0;
};

class IScoreBoard {
public:
    virtual ~IScoreBoard() = default;
    virtual bool onRaspberryPi() const = 0;
    virtual void setLittleDrawerFont(const std::string& /*fontPath*/) = 0;
    virtual void clearScreen() = 0;
    virtual void update() = 0;
};

class IRemotePairingScreen {
public:
    virtual ~IRemotePairingScreen() = default;
    virtual bool inPairingMode() const = 0;
    virtual void greenPlayerPressed() = 0;
    virtual void redPlayerPressed() = 0;
};

class IPairingBlinker {
public:
    virtual ~IPairingBlinker() = default;
    virtual void stop() = 0;
    virtual bool is_active() const = 0;
    virtual void setGreenPlayerPaired(bool) = 0;
    virtual void setRedPlayerPaired(bool) = 0;
};

class IScoreboardBlinker {
public:
    virtual ~IScoreboardBlinker() = default;
};

class IInputs {
public:
    virtual ~IInputs() = default;
    virtual int read_mcp23017_value() = 0; // for a real remote
};

class IInputWithTimer {
public:
    virtual ~IInputWithTimer() = default;
    virtual int getInput() = 0;
    virtual long getTimeSlept() const = 0;
};

class IGameObject {
public:
    virtual ~IGameObject() = default;

    virtual void loopGame() = 0;
    virtual void resetMatch() = 0;
    virtual void playerScore(int /*player*/) = 0;
    virtual void undo() = 0;

    virtual IScoreBoard* getScoreBoard() = 0;
    virtual IGameState* getGameState() = 0;

    class IHistory {
    public:
        virtual ~IHistory() = default;
        virtual void clearHistory() = 0;
    };
    virtual IHistory* getHistory() = 0;

    // For demonstration
    virtual int playerNotServing(int /*selection*/) = 0;  

    static void _signalHandler(int signal) { gSignalStatus = signal; }
};


/*******************************************
 * Stub "timer" utility to simulate game time
 *******************************************/
namespace GameTimer {
    inline unsigned long gameMillis() {
        using namespace std::chrono;
        static auto start = steady_clock::now();
        auto now = steady_clock::now();
        return (unsigned long)duration_cast<milliseconds>(now - start).count();
    }

    inline void gameDelay(unsigned long ms) {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }
}

/*******************************************
 * Minimal Implementation of IInputs for Keyboard 
 *******************************************/
class KeyboardInputs : public IInputs {
public:
    int read_mcp23017_value() override {
        // In real hardware code, you'd read from MCP23017.
        // For keyboard, just read an integer from std::cin.
        // If the user doesn't type anything, this might block,
        // so you may want a non-blocking read in real code.
        int sel;
        std::cout << "\nEnter your selection (1-4 = scores, 9 = undo): ";
        std::cin >> sel;
        return sel;
    }
};


/*******************************************
 * Stub classes to satisfy interface demands
 *******************************************/
class StubScoreBoard : public IScoreBoard {
public:
    bool onRaspberryPi() const override { return false; }
    void setLittleDrawerFont(const std::string&) override {}
    void clearScreen() override { std::cout << "[Scoreboard] clearScreen()\n"; }
    void update() override { std::cout << "[Scoreboard] update()\n"; }
};

class StubRemotePairingScreen : public IRemotePairingScreen {
public:
    bool _inPairingMode = true;
    bool _greenPressed = false;
    bool _redPressed = false;

    bool inPairingMode() const override { return _inPairingMode; }
    void greenPlayerPressed() override {
        std::cout << "[PairingScreen] Green Player Paired!\n";
        _greenPressed = true;
        if (_greenPressed && _redPressed) {
            _inPairingMode = false;
        }
    }
    void redPlayerPressed() override {
        std::cout << "[PairingScreen] Red Player Paired!\n";
        _redPressed = true;
        if (_greenPressed && _redPressed) {
            _inPairingMode = false;
        }
    }
};
class StubPairingBlinker : public IPairingBlinker {
public:
    bool _active = true;
    bool _greenPaired = false;
    bool _redPaired = false;

    void stop() override {
        _active = false; 
        std::cout << "[PairingBlinker] stop()\n";
    }
    bool is_active() const override {
        return _active;
    }
    void setGreenPlayerPaired(bool paired) override {
        _greenPaired = paired;
        std::cout << "[PairingBlinker] Green Paired: " << (_greenPaired ? "true" : "false") << "\n";
    }
    void setRedPlayerPaired(bool paired) override {
        _redPaired = paired;
        std::cout << "[PairingBlinker] Red Paired: " << (_redPaired ? "true" : "false") << "\n";
    }
};

class StubScoreboardBlinker : public IScoreboardBlinker {
public:
    StubScoreboardBlinker() {
        std::cout << "[ScoreboardBlinker] Created.\n";
    }
    ~StubScoreboardBlinker() {
        std::cout << "[ScoreboardBlinker] Destroyed.\n";
    }
};


/*******************************************
 * Example InputWithTimer to demonstrate timed read 
 * (Simplified from your original code)
 *******************************************/
class InputWithTimer : public IInputWithTimer {
public:
    InputWithTimer(IPairingBlinker* pairingBlinker, IInputs* inputs)
        : _blinker(pairingBlinker)
        , _inputs(inputs)
    {
        _timeStart = GameTimer::gameMillis();
    }
    ~InputWithTimer() override {}

    int getInput() override {
        // For demonstration, we read from _inputs (which might be keyboard or remote).
        int selection;
        std::cout << "\n[InputWithTimer] Please make a selection...\n";
        selection = _inputs->read_mcp23017_value();
        _timeEnd = GameTimer::gameMillis();
        _timeSlept = _timeEnd - _timeStart;
        return selection;
    }

    long getTimeSlept() const override {
        return _timeSlept;
    }

private:
    IPairingBlinker* _blinker{nullptr};
    IInputs* _inputs{nullptr};
    unsigned long _timeStart{0};
    unsigned long _timeEnd{0};
    long _timeSlept{0};
};

/*******************************************
 * Stub GameState - minimal state tracking
 *******************************************/
enum EnumGameState {
    PAIRING_MODE = 0,
    SLEEP_MODE,
    AFTER_SLEEP_MODE,
    GAME_ENDED
};

class StubGameState : public IGameState {
public:
    // For demonstration, let game run until we manually kill it
    bool _running = true;
    int  _currentAction = PAIRING_MODE;
    bool _anyScoreMade = false;
    bool _matchComplete = false;

    bool gameRunning() const override { return _running; }

    void setCurrentAction(int action) override {
        _currentAction = action;
    }
    int getCurrentAction() const override {
        return _currentAction;
    }

    // minimal stubs
    std::map<int,int> getPlayer1SetHistory() const override { return {}; }
    std::map<int,int> getPlayer2SetHistory() const override { return {}; }
    bool anyScoreMadeYet() const override { return _anyScoreMade; }
    bool matchComplete() const override { return _matchComplete; }
};

/*******************************************
 * Stub IHistory for demonstration
 *******************************************/
class StubHistory : public IGameObject::IHistory {
public:
    void clearHistory() override {
        std::cout << "[History] clearHistory()\n";
    }
};

/*******************************************
 * Stub GameObject
 *******************************************/
class StubGameObject : public IGameObject {
public:
    StubGameObject(IScoreBoard* sb, IGameState* gs)
        : _scoreBoard(sb), _gameState(gs)
    {
        _history = std::unique_ptr<StubHistory>(new StubHistory());
    }

    void loopGame() override {
        std::cout << "[GameObject] loopGame()\n";
        // Potentially update scoreboard, check conditions, etc.
        if (_scoreCountGreen >= 3) {
            std::cout << "[GameObject] Green wins match!\n";
            _gameState->setCurrentAction(SLEEP_MODE);
            static_cast<StubGameState*>(_gameState)->_matchComplete = true;
        } 
        if (_scoreCountRed >= 3) {
            std::cout << "[GameObject] Red wins match!\n";
            _gameState->setCurrentAction(SLEEP_MODE);
            static_cast<StubGameState*>(_gameState)->_matchComplete = true;
        }
    }

    void resetMatch() override {
        std::cout << "[GameObject] resetMatch()\n";
        _scoreCountGreen = 0;
        _scoreCountRed   = 0;
        static_cast<StubGameState*>(_gameState)->_matchComplete = false;
    }

    void playerScore(int player) override {
        // 1 = green, 2 = red, for example
        if (player == 1) {
            _scoreCountGreen++;
            std::cout << "[GameObject] Green scored. total = " << _scoreCountGreen << "\n";
        } else if (player == 2) {
            _scoreCountRed++;
            std::cout << "[GameObject] Red scored. total = " << _scoreCountRed << "\n";
        }
        static_cast<StubGameState*>(_gameState)->_anyScoreMade = true;
    }

    void undo() override {
        std::cout << "[GameObject] undo last scoring.\n";
        if (_scoreCountGreen > 0) {
            _scoreCountGreen--;
        } else if (_scoreCountRed > 0) {
            _scoreCountRed--;
        }
    }

    IScoreBoard* getScoreBoard() override {
        return _scoreBoard;
    }
    IGameState* getGameState() override {
        return _gameState;
    }
    IHistory* getHistory() override {
        return _history.get();
    }

    // Demo: returns non-zero if "player not serving"
    int playerNotServing(int /*selection*/) override {
        // For demonstration, always return 0 (meaning "serving is correct").
        // A real implementation would check which player should be serving, etc.
        return 0;
    }

private:
    IScoreBoard* _scoreBoard{nullptr};
    IGameState*  _gameState{nullptr};
    std::unique_ptr<StubHistory> _history{};

    int _scoreCountGreen{0};
    int _scoreCountRed{0};
};


/**************************************************************************************
 * Rewritten run_remote_listener function
 *
 * This version more directly manages the game states described:
 *  (1) Pairing Mode
 *  (2) Sleep Mode
 *  (3) Regular Game Play Before Scores (we call AFTER_SLEEP_MODE to represent it)
 *  (4) Regular Game Play After a Score (match or point is scored)
 *
 * The code demonstrates timeouts for transitions to Sleep Mode and from Sleep Mode.
 * It also shows how we might clear or preserve history after certain time thresholds.
 **************************************************************************************/
void run_remote_listener(IGameObject* gameObject,
                         IGameState* gameState,
                         /* We won't use this reset param here: */ 
                         void* /*reset*/,  
                         IInputs* inputs)
{
    std::cout << "[run_remote_listener] Entered.\n";

    // Initialize needed objects
    IScoreBoard* scoreboard = gameObject->getScoreBoard();
    scoreboard->setLittleDrawerFont("fonts/8x13B.bdf");

    std::signal(SIGINT, IGameObject::_signalHandler);

    // Pairing Screen & Blinker
    IRemotePairingScreen* remotePairingScreen = new StubRemotePairingScreen();
    IPairingBlinker* pairingBlinker = new StubPairingBlinker();

    // This inputWithTimer is used only during pairing
    IInputWithTimer* pairingInputTimer = new InputWithTimer(pairingBlinker, inputs);

    // We'll track the start of Pairing Mode to handle the 10s timeout
    auto pairingStart = GameTimer::gameMillis();

    // Main loop
    while (gameState->gameRunning() && gSignalStatus != SIGINT)
    {
        // Check the current state from gameState
        int currentAction = gameState->getCurrentAction();

        // 1) PAIRING MODE
        if (currentAction == PAIRING_MODE)
        {
            std::cout << "[run_remote_listener] State: PAIRING_MODE\n";

            // Check if we exceeded 10 seconds in Pairing
            unsigned long now = GameTimer::gameMillis();
            if ((now - pairingStart) > (MAX_SLEEP * 1000))
            {
                std::cout << "[run_remote_listener] Pairing timed out. Going to Sleep Mode.\n";
                // Stop the pairing blinker
                pairingBlinker->stop();
                // Move to Sleep Mode
                gameState->setCurrentAction(SLEEP_MODE);
                continue;
            }

            // If still in pairing mode, we wait for input
            int selection = pairingInputTimer->getInput();

            // Evaluate selection
            if (selection == GREEN_REMOTE_GREEN_SCORE)
            {
                remotePairingScreen->greenPlayerPressed();
                pairingBlinker->setGreenPlayerPaired(true);
            }
            else if (selection == RED_REMOTE_RED_SCORE)
            {
                remotePairingScreen->redPlayerPressed();
                pairingBlinker->setRedPlayerPaired(true);
            }
            else 
            {
                std::cout << "[run_remote_listener] Invalid selection during pairing.\n";
                // short delay, then keep waiting
                GameTimer::gameDelay(500);
            }

            // If the pairing screen is no longer in pairing mode or both are paired,
            // we can transition to "Regular Game Play (Before Scores)".
            if (!remotePairingScreen->inPairingMode())
            {
                std::cout << "[run_remote_listener] Pairing complete! Transitioning to REGULAR_BEFORE_SCORES.\n";
                pairingBlinker->stop();
                gameState->setCurrentAction(AFTER_SLEEP_MODE); 
                // AFTER_SLEEP_MODE is used to represent "Regular Game Play Before Scores" in this stub.
            }
        }

        // 2) SLEEP MODE
        else if (currentAction == SLEEP_MODE)
        {
            std::cout << "[run_remote_listener] State: SLEEP_MODE\n";
            // Create a scoreboard blinker for Sleep Mode
            IScoreboardBlinker* sleepBlinker = new StubScoreboardBlinker();

            // We'll read one input with timer to see how long we waited
            IInputWithTimer* sleepInputTimer = new InputWithTimer(nullptr, inputs);
            int selection = sleepInputTimer->getInput();
            long timeAsleepMs = sleepInputTimer->getTimeSlept();

            std::cout << "[run_remote_listener] Woke from Sleep Mode after " 
                      << timeAsleepMs << " ms.\n";

            // If user pressed a scoring button or if we were in Sleep > 10s:
            // then either reset match or keep it, per the instructions
            if ((selection == GREEN_REMOTE_GREEN_SCORE ||
                 selection == GREEN_REMOTE_RED_SCORE ||
                 selection == RED_REMOTE_GREEN_SCORE ||
                 selection == RED_REMOTE_RED_SCORE) ||
                (timeAsleepMs > MAX_SLEEP * 1000))
            {
                // If Sleep was > 10 seconds, clear history
                if (timeAsleepMs > MAX_SLEEP * 1000) {
                    std::cout << "[run_remote_listener] Sleep exceeded 10s, clearing history.\n";
                    gameObject->getHistory()->clearHistory();
                }

                // We also reset the match if needed. The instructions mention 
                // resuming the match if within 10s, but let's do a simple approach:
                // If we want to keep the logic that pressing a score within 10s 
                // resumes the match *with* history, we can adapt. 
                // For demonstration, let's do a reset after > 10s; otherwise just continue.
                if (timeAsleepMs > MAX_SLEEP * 1000)
                {
                    std::cout << "[run_remote_listener] Resetting match due to exceeding sleep time.\n";
                    gameObject->resetMatch();
                }

                // Move to "Regular Game Play (Before Scores)"
                gameState->setCurrentAction(AFTER_SLEEP_MODE);
            }
            else 
            {
                // If user pressed Undo or something else, we still wake up. 
                // The instructions say "any input" exits Sleep Mode => "Regular Game Play".
                gameState->setCurrentAction(AFTER_SLEEP_MODE);
            }

            delete sleepBlinker;
            delete sleepInputTimer;
        }

        // 3) REGULAR GAME PLAY (BEFORE SCORES) => AFTER_SLEEP_MODE 
        //    or 4) AFTER A SCORE => We'll just unify them for demonstration.
        else if (currentAction == AFTER_SLEEP_MODE)
        {
            // If match is already complete, pressing Undo might revert if under 10s, etc.
            // We'll track time at the start of AFTER_SLEEP_MODE:
            static unsigned long afterSleepEntryTime = GameTimer::gameMillis();
            bool matchDone = gameState->matchComplete();

            // If the match is done, the instructions mention we can re-enter the game if within 10s,
            // otherwise we might clear. We'll do a quick check:
            if (matchDone) {
                std::cout << "[run_remote_listener] Match was complete. Press a button to resume or Undo.\n";
            } else {
                std::cout << "[run_remote_listener] State: REGULAR_GAME_PLAY (BEFORE SCORES or AFTER SCORE)\n";
            }

            // We'll do a quick idle check: if no input for 10s => Sleep Mode
            // The simplest approach is to do a timed read for input:
            IInputWithTimer* gamePlayInputTimer = new InputWithTimer(nullptr, inputs);
            int selection = gamePlayInputTimer->getInput();
            long idleMs = gamePlayInputTimer->getTimeSlept();

            if (idleMs > MAX_SLEEP * 1000)
            {
                std::cout << "[run_remote_listener] Idle in Regular Game for > 10s => Sleep Mode.\n";
                gameState->setCurrentAction(SLEEP_MODE);
                delete gamePlayInputTimer;
                continue;
            }

            // We have some input. Let's handle it:
            std::cout << "[run_remote_listener] Input received: " << selection << "\n";

            // Check serve_flag (playerNotServing)
            if (gameObject->playerNotServing(selection) != 0) {
                std::cout << "[run_remote_listener] Player not serving! Ignoring.\n";
                delete gamePlayInputTimer;
                continue;
            }

            // Evaluate if it's a score or an undo
            if (selection == GREEN_REMOTE_GREEN_SCORE || selection == RED_REMOTE_GREEN_SCORE)
            {
                std::cout << "[run_remote_listener] Green player scored.\n";
                gameObject->playerScore(1);
            }
            else if (selection == GREEN_REMOTE_RED_SCORE || selection == RED_REMOTE_RED_SCORE)
            {
                std::cout << "[run_remote_listener] Red player scored.\n";
                gameObject->playerScore(2);
            }
            else if (selection == GREEN_REMOTE_UNDO || selection == RED_REMOTE_UNDO)
            {
                std::cout << "[run_remote_listener] Undo pressed.\n";
                gameObject->undo();
            }
            else {
                std::cout << "[run_remote_listener] Invalid selection.\n";
            }

            // Let the game handle any new scoring logic
            gameObject->loopGame();

            delete gamePlayInputTimer;
        }
        else 
        {
            // If some unknown state or the game ended, we can break:
            std::cout << "[run_remote_listener] Unknown or ended state, exiting...\n";
            break;
        }

        // short artificial delay to avoid busy spinning
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    // Cleanup
    delete remotePairingScreen;
    delete pairingBlinker;
    delete pairingInputTimer;

    std::cout << "[run_remote_listener] Exiting game loop.\n";
}


/*******************************************
 * MAIN FUNCTION
 * We create stubs and run the system with
 * a Keyboard-based input approach. 
 * You can seamlessly swap in an MCP23017
 * remote-based input class in place of 
 * KeyboardInputs if desired.
 *******************************************/
int main()
{
    // Create stub scoreboard and game state
    StubScoreBoard sb;
    StubGameState  gs;

    // Create game object
    StubGameObject gameObj(&sb, &gs);

    // Create keyboard-based inputs
    KeyboardInputs kbInput;

    // Run the remote listener logic
    run_remote_listener(&gameObj, &gs, nullptr, &kbInput);

    std::cout << "[main] Exiting program.\n";
    return 0;
}
