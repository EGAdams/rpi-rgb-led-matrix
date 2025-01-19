Please create a Mermaid Sequence Diagram for the following code.  I am trying to focus on how the different Interfaces are interacting with each other.  Here is the code:
```cpp
/************************************************************
 *  COMBINED SINGLE-FILE EXAMPLE
 *
 *  This file merges:
 *    1) The "Input With Timer Code" (first snippet),
 *    2) The "System that uses the Input With Timer Code"
 *       (second snippet),
 *  into a single .cpp file with one active main().
 *
 *  HOW TO COMPILE/RUN (example):
 *    g++ -std=c++17 combined.cpp -o combined && ./combined
 *
 *  NOTES:
 *  - The first snippet's classes/functions are renamed slightly
 *    (e.g., Blinker -> BlinkerDemo, Inputs -> InputsDemo,
 *    InputWithTimer -> InputWithTimerDemo) to avoid collisions
 *    with the second snippet's classes.
 *  - The first snippet's main() is commented out with #if 0 ... #endif.
 *    The active main() is the one from the second snippet, which
 *    calls run_remote_listener().
 *  - Both snippets now coexist in one translation unit without
 *    duplicate symbol conflicts.
 ************************************************************/

#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <atomic>
#include <csignal>
#include <map>
#include <memory>

/*==========================================================
 *  A simple global "print" function
 *==========================================================*/
static void print(const std::string &msg) {
    std::cout << msg << std::endl;
}

/*==========================================================
 *  Combined Constants
 *==========================================================*/
static const int GREEN_REMOTE_GREEN_SCORE = 1;
static const int GREEN_REMOTE_RED_SCORE   = 2;
static const int RED_REMOTE_GREEN_SCORE   = 3;
static const int RED_REMOTE_RED_SCORE     = 4;
static const int GREEN_REMOTE_UNDO        = 5;
static const int RED_REMOTE_UNDO          = 6;

// 0 => local "menu mode" input (std::cin), 1 => remote hardware read
static const int REMOTE_INPUT = 0;

static const int SLEEP_MODE      = 100; 
static const int AFTER_SLEEP_MODE= 101;
static const int MAX_SLEEP       = 60;   // in seconds
static const int SCORE_DELAY     = 1;    // in seconds

/*==========================================================
 *  Combined GameTimer with both gameDelay() and gameMillis()
 *==========================================================*/
class GameTimer {
public:
    // Return current time in milliseconds since epoch
    static unsigned long gameMillis() {
        using namespace std::chrono;
        auto now = system_clock::now().time_since_epoch();
        return static_cast<unsigned long>(
            duration_cast<milliseconds>( now ).count()
        );
    }
    // Sleep for the specified number of milliseconds
    static void gameDelay(int ms) {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }
};

/*==========================================================
 *  Interface Declarations
 *==========================================================*/
class IGameState {
public:
    virtual ~IGameState() = default;
    virtual bool gameRunning() const = 0;
    virtual int  getCurrentAction() const = 0;
    virtual void setCurrentAction(int action) = 0;
    virtual std::map<int, int> getPlayer1SetHistory() const = 0;
    virtual std::map<int, int> getPlayer2SetHistory() const = 0;
};

class IReset {
public:
    virtual ~IReset() = default;
};

class IInputs {
public:
    virtual ~IInputs() = default;
    virtual int read_mcp23017_value() = 0;
};

class IScoreBoard {
public:
    virtual ~IScoreBoard() = default;
    virtual void setLittleDrawerFont(const std::string &font) = 0;
    virtual bool onRaspberryPi() const = 0;
    virtual void clearScreen() = 0;
    virtual void update() = 0;
};

class IHistory {
public:
    virtual ~IHistory() = default;
    virtual void clearHistory() = 0;
};

class IGameObject {
public:
    virtual ~IGameObject() = default;

    virtual void loopGame() = 0;
    virtual std::shared_ptr<IScoreBoard> getScoreBoard() = 0;
    virtual std::shared_ptr<IHistory> getHistory() = 0;

    virtual void resetMatch() = 0;
    virtual void undo() = 0;
    virtual void playerScore(int whichPlayer) = 0;

    static void _signalHandler(int signal) {
        std::cout << "IGameObject::_signalHandler called with signal: "
                  << signal << std::endl;
    }
};

class IRemoteLocker {
public:
    virtual ~IRemoteLocker() = default;
    virtual int playerNotServing(int selection) = 0;
};

class IRemotePairingScreen {
public:
    virtual ~IRemotePairingScreen() = default;
    virtual bool inPairingMode() const = 0;
    virtual void greenPlayerPressed() = 0;
    virtual void redPlayerPressed() = 0;
};

class IBlinker {
public:
    virtual ~IBlinker() = default;
    virtual void start() = 0;
    virtual void stop() = 0;
};

class IPairingBlinker {
public:
    virtual ~IPairingBlinker() = default;
    virtual bool awake() const = 0;
    virtual void stop() = 0;
    virtual void setGreenPlayerPaired(bool paired) = 0;
    virtual void setRedPlayerPaired(bool paired) = 0;
};

class IScoreboardBlinker {
public:
    virtual ~IScoreboardBlinker() = default;
};

class IInputWithTimer {
public:
    virtual ~IInputWithTimer() = default;
    virtual int  getInput() = 0;
    virtual unsigned long getTimeSlept() = 0;
};

/*==========================================================
 *  FIRST SNIPPET CLASSES (Renamed to avoid collisions)
 *==========================================================*/

// A "Blinker" from first snippet, renamed to BlinkerDemo
class BlinkerDemo : public IBlinker {
public:
    void start() override {
        print("BlinkerDemo: start()");
    }
    void stop() override {
        print("BlinkerDemo: stop()");
    }
};

// A "Inputs" from first snippet, renamed to InputsDemo
class InputsDemo : public IInputs {
public:
    int read_mcp23017_value() override {
        // Return a cycling sequence of remote codes
        static int calls = 0;
        calls++;
        switch (calls % 5) {
            case 0: return GREEN_REMOTE_GREEN_SCORE;
            case 1: return GREEN_REMOTE_RED_SCORE;
            case 2: return RED_REMOTE_GREEN_SCORE;
            case 3: return RED_REMOTE_RED_SCORE;
            default: return 99; // invalid
        }
    }
};

// The refactored InputWithTimer from first snippet, renamed to InputWithTimerDemo
class InputWithTimerDemo : public IInputWithTimer {
public:
    InputWithTimerDemo(IBlinker* blinker, IInputs* inputs)
        : _blinker(blinker), _inputs(inputs), _time_slept(0)
    {}

    int getInput() override {
        unsigned long sleep_start = GameTimer::gameMillis();
        int selection = 0;
        bool done = false;

        print("starting blinker from within InputWithTimerDemo...");
        _blinker->start();

        print("getting input from within InputWithTimerDemo...");
        if (REMOTE_INPUT == 1) {
            while (!done) {
                print("*** reading selection from inputs... ***");
                selection = _inputs->read_mcp23017_value();
                std::cout << "read selection from inputs: " << selection << std::endl;
                if ( selection == GREEN_REMOTE_GREEN_SCORE ||
                     selection == GREEN_REMOTE_RED_SCORE   ||
                     selection == RED_REMOTE_GREEN_SCORE   ||
                     selection == RED_REMOTE_RED_SCORE )
                {
                    std::cout << "selection: " << selection
                              << " triggered the done flag, exiting while loop..."
                              << std::endl;
                    done = true;
                } else {
                    std::cout << "sleeping 250ms..." << std::endl;
                    GameTimer::gameDelay(250);
                }
            }
        } else if (REMOTE_INPUT == 0) {
            std::cout << "Enter your selection: ";
            std::cin >> selection;
            print("made selection in InputWithTimerDemo::getInput()...");
            print("selection: " + std::to_string(selection));
        } else {
            std::cout << "*** ERROR: Invalid input mode in InputWithTimerDemo::getInput() ***\n";
            exit(1);
        }

        _blinker->stop();
        unsigned long sleep_end = GameTimer::gameMillis();
        _time_slept = sleep_end - sleep_start;

        return selection;
    }

    unsigned long getTimeSlept() override {
        return _time_slept;
    }

private:
    IBlinker*     _blinker;
    IInputs*      _inputs;
    unsigned long _time_slept;
};

/* 
 * (Optional) A small test main for the first snippet.
 * It is commented out so we can use the second snippet's main.
 */
#if 0
int main_demo() {
    BlinkerDemo myBlinker;
    InputsDemo myInputs;
    InputWithTimerDemo inputWithTimer(&myBlinker, &myInputs);

    int result = inputWithTimer.getInput();
    std::cout << "Result of getInput(): " << result << std::endl;
    std::cout << "Time slept (ms): " << inputWithTimer.getTimeSlept() << std::endl;

    return 0;
}
#endif

/*==========================================================
 *  SECOND SNIPPET CLASSES
 *==========================================================*/

// Example concrete RemoteLocker
class RemoteLocker : public IRemoteLocker {
public:
    RemoteLocker(std::shared_ptr<IGameState> state) : m_gameState(state) {}
    
    int playerNotServing(int selection) override {
        // Minimal example: always return 0 if valid
        (void)selection;
        return 0; 
    }

private:
    std::shared_ptr<IGameState> m_gameState;
};

// Example concrete RemotePairingScreen
class RemotePairingScreen : public IRemotePairingScreen {
public:
    RemotePairingScreen(std::shared_ptr<IScoreBoard> scoreboard)
        : m_scoreboard(scoreboard), m_inPairingMode(true) {}

    bool inPairingMode() const override {
        return m_inPairingMode;
    }
    void greenPlayerPressed() override {
        print("Green player paired on RemotePairingScreen");
        m_inPairingMode = false;
    }
    void redPlayerPressed() override {
        print("Red player paired on RemotePairingScreen");
        m_inPairingMode = false;
    }

private:
    std::shared_ptr<IScoreBoard> m_scoreboard;
    bool m_inPairingMode;
};

// Example concrete PairingBlinker
class PairingBlinker : public IPairingBlinker {
public:
    PairingBlinker(std::shared_ptr<IScoreBoard> scoreboard)
        : m_scoreboard(scoreboard), m_awake(true),
          m_greenPaired(false), m_redPaired(false) {}

    bool awake() const override {
        return m_awake;
    }
    void stop() override {
        m_awake = false;
    }
    void setGreenPlayerPaired(bool paired) override {
        m_greenPaired = paired;
        print("PairingBlinker: Green player paired? " + std::to_string(m_greenPaired));
    }
    void setRedPlayerPaired(bool paired) override {
        m_redPaired = paired;
        print("PairingBlinker: Red player paired? " + std::to_string(m_redPaired));
    }

private:
    std::shared_ptr<IScoreBoard> m_scoreboard;
    bool m_awake;
    bool m_greenPaired;
    bool m_redPaired;
};

// Example concrete ScoreboardBlinker
class ScoreboardBlinker : public IScoreboardBlinker {
public:
    ScoreboardBlinker(std::shared_ptr<IScoreBoard> scoreboard)
        : m_scoreboard(scoreboard) {}

private:
    std::shared_ptr<IScoreBoard> m_scoreboard;
};

/*
 * The second snippet's InputWithTimer (different from the first snippet's).
 * It implements IInputWithTimer, but uses a PairingBlinker (IPairingBlinker).
 */
class InputWithTimer : public IInputWithTimer {
public:
    InputWithTimer(std::shared_ptr<IPairingBlinker> blinker,
                   std::shared_ptr<IInputs> inputs)
        : m_blinker(blinker), m_inputs(inputs), m_timeSlept(0) {}

    int getInput() override {
        using namespace std::chrono;
        auto start = steady_clock::now();

        int value = 0;
        if (REMOTE_INPUT == 1) {
            value = m_inputs->read_mcp23017_value();
        } else {
            std::cout << "Enter a value: ";
            std::cin >> value;
        }

        auto end = steady_clock::now();
        m_timeSlept = duration_cast<milliseconds>(end - start).count();
        return value;
    }

    unsigned long getTimeSlept() override {
        return static_cast<unsigned long>(m_timeSlept);
    }

private:
    std::shared_ptr<IPairingBlinker> m_blinker;
    std::shared_ptr<IInputs>         m_inputs;
    long                             m_timeSlept;
};

// Example minimal ScoreBoard
class ScoreBoard : public IScoreBoard {
public:
    void setLittleDrawerFont(const std::string &font) override {
        print("ScoreBoard: setting font to " + font);
    }
    bool onRaspberryPi() const override {
        return false;
    }
    void clearScreen() override {
        print("ScoreBoard: clearScreen()");
    }
    void update() override {
        print("ScoreBoard: update()");
    }
};

// Example minimal History
class History : public IHistory {
public:
    void clearHistory() override {
        print("History: cleared");
    }
};

// Example minimal GameState
class GameState : public IGameState {
public:
    bool gameRunning() const override {
        return true; // always keep running in this demo
    }
    int getCurrentAction() const override {
        return m_currentAction;
    }
    void setCurrentAction(int action) override {
        m_currentAction = action;
    }
    std::map<int,int> getPlayer1SetHistory() const override {
        return m_p1History;
    }
    std::map<int,int> getPlayer2SetHistory() const override {
        return m_p2History;
    }

private:
    int m_currentAction = AFTER_SLEEP_MODE;
    std::map<int,int> m_p1History;
    std::map<int,int> m_p2History;
};

// Example minimal GameObject
class GameObject : public IGameObject {
public:
    GameObject() {
        m_scoreboard = std::make_shared<ScoreBoard>();
        m_history    = std::make_shared<History>();
    }
    void loopGame() override {
        print("GameObject: loopGame()");
    }
    std::shared_ptr<IScoreBoard> getScoreBoard() override {
        return m_scoreboard;
    }
    std::shared_ptr<IHistory> getHistory() override {
        return m_history;
    }
    void resetMatch() override {
        print("GameObject: resetMatch()");
    }
    void undo() override {
        print("GameObject: undo()");
    }
    void playerScore(int whichPlayer) override {
        print("GameObject: playerScore(" + std::to_string(whichPlayer) + ")");
    }

private:
    std::shared_ptr<IScoreBoard> m_scoreboard;
    std::shared_ptr<IHistory>    m_history;
};

// Example minimal Reset
class Reset : public IReset {
};

// Example minimal Inputs
class Inputs : public IInputs {
public:
    int read_mcp23017_value() override {
        // Always return 42 in this demo
        return 42;
    }
};

/*==========================================================
 *  Global signal variable
 *==========================================================*/
static volatile std::sig_atomic_t gSignalStatus = 0;

/*==========================================================
 *  The Refactored run_remote_listener() from snippet #2
 *==========================================================*/
void run_remote_listener(IGameObject* gameObject,
                         IGameState*  gameState,
                         IReset* /*unused*/,
                         IInputs*     inputs)
{
    std::shared_ptr<IGameState> spGameState(
        dynamic_cast<IGameState*>(gameState) ? gameState : nullptr
    );

    std::unique_ptr<IRemoteLocker> remoteLocker =
        std::make_unique<RemoteLocker>(spGameState);

    print("entered run remote listener method...");
    int loop_count = 0;
    int selection = 0;
    print("calling game object loop game...");
    gameObject->loopGame();
    std::this_thread::sleep_for(std::chrono::seconds(1));

    auto scoreboard = gameObject->getScoreBoard();
    scoreboard->setLittleDrawerFont("fonts/8x13B.bdf");

    std::signal(SIGINT, IGameObject::_signalHandler);

    print("constructing remote pairing screen...");
    std::unique_ptr<IRemotePairingScreen> remotePairingScreen =
        std::make_unique<RemotePairingScreen>(scoreboard);

    print("constructing pairing blinker...");
    std::shared_ptr<IPairingBlinker> pairingBlinker =
        std::make_shared<PairingBlinker>(scoreboard);

    print("constructing input with timer...");
    std::unique_ptr<IInputWithTimer> inputWithTimer =
        std::make_unique<InputWithTimer>(
            pairingBlinker,
            std::shared_ptr<IInputs>(inputs, [](IInputs*){})
        );
    print("finished constructing input with timer...");

    bool is_on_pi = scoreboard->onRaspberryPi();
    print("is_on_pi: " + std::to_string(is_on_pi));

    while (gameState->gameRunning() && gSignalStatus != SIGINT) {
        std::this_thread::sleep_for(std::chrono::seconds(SCORE_DELAY));

        // if remote pairing, remain in that mode until paired
        while (remotePairingScreen->inPairingMode() && pairingBlinker->awake()) {
            print("inside remote pairing screen....  before starting input timer...");
            if (REMOTE_INPUT == 1) {
                selection = inputWithTimer->getInput();
            } else {
                std::cin >> selection;
                print("*** inside remote listener getting remote selection ***");
                print("selection: " + std::to_string(selection));
            }

            if (selection == GREEN_REMOTE_GREEN_SCORE) {
                remotePairingScreen->greenPlayerPressed();
                pairingBlinker->setGreenPlayerPaired(true);
            }
            else if (selection == RED_REMOTE_RED_SCORE) {
                remotePairingScreen->redPlayerPressed();
                pairingBlinker->setRedPlayerPaired(true);
            }
            else {
                std::cout << "*** Invalid selection during remote pairing. ***\n";
                GameTimer::gameDelay(1000);
            }
        }

        if (!pairingBlinker->awake()) {
            print("pairing blinker is not awake, stopping it... ");
            pairingBlinker->stop();
            print("pairing blinker stopped.  now putting in sleep mode...");
            gameState->setCurrentAction(SLEEP_MODE);
        }

        if (gameState->getCurrentAction() == SLEEP_MODE) {
            print("current action is SLEEP_MODE");
            std::unique_ptr<IScoreboardBlinker> blinker =
                std::make_unique<ScoreboardBlinker>(scoreboard);

            std::unique_ptr<IInputWithTimer> inputWithTimer2 =
                std::make_unique<InputWithTimer>(
                    std::shared_ptr<IPairingBlinker>(),
                    std::shared_ptr<IInputs>(inputs, [](IInputs*){})
                );
            int selection = inputWithTimer2->getInput();

            gameState->setCurrentAction(AFTER_SLEEP_MODE);
            std::cout << "time slept: " << inputWithTimer2->getTimeSlept() << std::endl;

            if (selection == GREEN_REMOTE_GREEN_SCORE ||
                selection == GREEN_REMOTE_RED_SCORE   ||
                (inputWithTimer2->getTimeSlept() > MAX_SLEEP * 1000))
            {
                print("reset match.");
                gameObject->resetMatch();
                print("done resetting match.");
                if (inputWithTimer2->getTimeSlept() > MAX_SLEEP * 1000) {
                    print("time slept: " +
                          std::to_string(inputWithTimer2->getTimeSlept() / 1000) +
                          " seconds.");
                    print("clearing History because max sleep time has been reached or exceeded.");
                    gameObject->getHistory()->clearHistory();
                    print("done clearing history because max sleep time has been reached or exceeded.");
                }
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
        }
        else {
            if (REMOTE_INPUT == 0) {
                std::cin >> selection;
                print("selection: " + std::to_string(selection));
            } else {
                bool done = false;
                while (!done) {
                    selection = inputs->read_mcp23017_value();
                    std::cout << "read selection from inputs: " << selection << std::endl;
                    if ( selection == GREEN_REMOTE_GREEN_SCORE ||
                         selection == GREEN_REMOTE_RED_SCORE   ||
                         selection == RED_REMOTE_GREEN_SCORE   ||
                         selection == RED_REMOTE_RED_SCORE     ||
                         selection == GREEN_REMOTE_UNDO        ||
                         selection == RED_REMOTE_UNDO )
                    {
                        std::cout << "selection: " << selection
                                  << " triggered the done flag, exiting while loop..."
                                  << std::endl;
                        done = true;
                    } else {
                        std::cout << "sleeping 250ms..." << std::endl;
                        GameTimer::gameDelay(250);
                    }
                }
            }
        }

        int serve_flag = remoteLocker->playerNotServing(selection);
        print("*** serve_flag: " + std::to_string(serve_flag) + " ***");
        if (serve_flag) {
            print("*** Warning: player not serving! ***");
            continue;
        }

        print("setting player button to selection: " + std::to_string(selection) +
              " before calling loopGame()...");

        if (selection == GREEN_REMOTE_GREEN_SCORE ||
            selection == GREEN_REMOTE_RED_SCORE   ||
            selection == RED_REMOTE_GREEN_SCORE   ||
            selection == RED_REMOTE_RED_SCORE)
        {
            if (selection == GREEN_REMOTE_GREEN_SCORE ||
                selection == RED_REMOTE_GREEN_SCORE)
            {
                print("*** \n\n\nGreen player scored ***\n\n\n");
                selection = 1; // represent GREEN
            } else {
                print("\n\n\n*** Red player scored ***\n\n\n");
                selection = 2; // represent RED
            }
            gameObject->playerScore(selection);
        }
        else if (selection == GREEN_REMOTE_UNDO ||
                 selection == RED_REMOTE_UNDO)
        {
            print("\n\n\n*** Undo ***\n\n\n");
            gameObject->undo();
        }
        else {
            std::cout << "\n\n\n*** Invalid selection ***\n\n\n" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(SCORE_DELAY));
            continue;
        }

        std::this_thread::sleep_for(std::chrono::seconds(SCORE_DELAY));
        gameObject->loopGame();
        loop_count++;

        // Mimic the original code's retrieval of set history
        std::map<int,int> _player1_set_history = gameState->getPlayer1SetHistory();
        std::map<int,int> _player2_set_history = gameState->getPlayer2SetHistory();
        (void)_player1_set_history;
        (void)_player2_set_history;
    }
}

/*==========================================================
 *  The Active main() from snippet #2
 *==========================================================*/
int main() {
    GameObject myGameObject;
    GameState  myGameState;
    Reset      myReset;
    Inputs     myInputs;

    run_remote_listener(&myGameObject, &myGameState, &myReset, &myInputs);
    return 0;
}
```