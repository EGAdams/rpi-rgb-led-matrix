# Persona
You are an AI debugger who is trying to fully describe a program, in order for another AI program to reconstruct every file, data structure, function and functionality.
  
# Source Code To Describe
```cpp
#include <iostream>
#include <string>
#include <map>
#include <memory>
#include <csignal>
#include <thread>
#include <chrono>

/*==========================================================
 *  Constants & Utilities (stubs to keep code self-contained)
 *==========================================================*/
static const int GREEN_REMOTE_GREEN_SCORE = 1;
static const int GREEN_REMOTE_RED_SCORE   = 2;
static const int RED_REMOTE_GREEN_SCORE   = 3;
static const int RED_REMOTE_RED_SCORE     = 4;
static const int GREEN_REMOTE_UNDO        = 5;
static const int RED_REMOTE_UNDO          = 6;

static const int REMOTE_INPUT = 0;  // 0 => local input, 1 => remote input

static const int SLEEP_MODE      = 100; 
static const int AFTER_SLEEP_MODE= 101;
static const int MAX_SLEEP       = 60;   // in seconds
static const int SCORE_DELAY     = 1;    // in seconds

// Simple print-like function to keep the style of the original
void print( const std::string &msg ) {
    std::cout << msg << std::endl;
}

// Example "GameTimer" with static delay function
class GameTimer {
public:
    static void gameDelay( int ms ) {
        std::this_thread::sleep_for( std::chrono::milliseconds( ms ));
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
    virtual void setCurrentAction( int action ) = 0;
    virtual std::map<int, int> getPlayer1SetHistory() const = 0;
    virtual std::map<int, int> getPlayer2SetHistory() const = 0;
};

class IReset {
public:
    virtual ~IReset() = default;
    // Example: if "Reset" has methods, add them here
};

class IInputs {
public:
    virtual ~IInputs() = default;
    virtual int read_mcp23017_value() = 0;
};

class IScoreBoard {
public:
    virtual ~IScoreBoard() = default;
    virtual void setLittleDrawerFont( const std::string &font ) = 0;
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
    virtual void playerScore( int whichPlayer ) = 0;
    
    // The original code had a static signal handler
    static void _signalHandler( int signal ) {
        std::cout << "IGameObject::_signalHandler called with signal: " << signal << std::endl;
        print( "Received signal, exiting..." );
        exit( 0 );
    }
};

class IRemoteLocker {
public:
    virtual ~IRemoteLocker() = default;
    virtual int playerNotServing( int selection ) = 0;
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
    virtual bool awake() const = 0;
    virtual void stop() = 0;
    virtual void setGreenPlayerPaired( bool paired ) = 0;
    virtual void setRedPlayerPaired( bool paired ) = 0;
};

class IScoreboardBlinker {
public:
    virtual ~IScoreboardBlinker() = default;
};

class IInputWithTimer {
public:
    virtual ~IInputWithTimer() = default;
    virtual int  getInput() = 0;
    virtual long getTimeSlept() const = 0;
};

/*==========================================================
 *  Concrete Implementations (stubs)
 *
 *  In a real project, these would be in separate .cpp files
 *  implementing the interfaces. Shown here just so that the
 *  run_remote_listener() example can be complete.
 *==========================================================*/

// Example concrete RemoteLocker
class RemoteLocker : public IRemoteLocker {
public:
    RemoteLocker( std::shared_ptr<IGameState> state )
        : m_gameState( state ) {}
    
    int playerNotServing( int selection ) override {
        // Minimal example: always return 0 if valid, 1 if invalid
        // You would implement your real logic here
        (void)selection;
        return 0; 
    }

private:
    std::shared_ptr<IGameState> m_gameState;
};

// Example concrete RemotePairingScreen
class RemotePairingScreen : public IRemotePairingScreen {
public:
    RemotePairingScreen( std::shared_ptr<IScoreBoard> scoreboard )
        : m_scoreboard( scoreboard ), m_inPairingMode( true ) {}

    bool inPairingMode() const override {
        return m_inPairingMode;
    }

    void greenPlayerPressed() override {
        // Logic for green player pairing
        print( "Green player paired on RemotePairingScreen" );
        m_inPairingMode = false; // for example
    }

    void redPlayerPressed() override {
        // Logic for red player pairing
        print( "Red player paired on RemotePairingScreen" );
        m_inPairingMode = false; // for example
    }

private:
    std::shared_ptr<IScoreBoard> m_scoreboard;
    bool m_inPairingMode;
};

// Example concrete PairingBlinker
class PairingBlinker : public IPairingBlinker {
public:
    PairingBlinker( std::shared_ptr<IScoreBoard> scoreboard )
        : m_scoreboard( scoreboard ), m_awake( true ),
          m_greenPaired( false ), m_redPaired( false ) {}

    bool awake() const override {
        return m_awake;
    }

    void stop() override {
        m_awake = false;
    }

    void setGreenPlayerPaired( bool paired ) override {
        m_greenPaired = paired;
        print( "PairingBlinker: Green player paired? " + std::to_string( m_greenPaired ));
    }

    void setRedPlayerPaired( bool paired ) override {
        m_redPaired = paired;
        print( "PairingBlinker: Red player paired? " + std::to_string( m_redPaired ));
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
    ScoreboardBlinker( std::shared_ptr<IScoreBoard> scoreboard )
        : m_scoreboard( scoreboard ) {
    }
private:
    std::shared_ptr<IScoreBoard> m_scoreboard;
};

// Example concrete InputWithTimer
class InputWithTimer : public IInputWithTimer {
public:
    InputWithTimer( std::shared_ptr<IPairingBlinker> blinker,
                    std::shared_ptr<IInputs> inputs )
        : m_blinker( blinker ), m_inputs( inputs ), m_timeSlept( 0 ) {}

    int getInput() override {
        // For a real implementation, you'd start a timer thread,
        // watch for input, etc. We'll keep it simple:
        using namespace std::chrono;

        auto start = steady_clock::now();
        int value = 0;
        if ( REMOTE_INPUT == 1 ) {
            // Suppose we read from the inputs interface
            value = m_inputs->read_mcp23017_value();
        } else {
            std::cout << "Enter a value: ";
            std::cin >> value; // local
        }
        auto end = steady_clock::now();

        m_timeSlept = duration_cast<milliseconds>( end - start ).count();
        return value;
    }

    long getTimeSlept() const override {
        return m_timeSlept;
    }

private:
    std::shared_ptr<IPairingBlinker> m_blinker;
    std::shared_ptr<IInputs>         m_inputs;
    long                             m_timeSlept;
};

// Example minimal scoreboard
class ScoreBoard : public IScoreBoard {
public:
    void setLittleDrawerFont( const std::string &font ) override {
        print( "ScoreBoard: setting font to " + font );
    }
    bool onRaspberryPi() const override {
        // Return true/false based on your environment
        return false;
    }
    void clearScreen() override {
        print( "ScoreBoard: clearScreen()" );
    }
    void update() override {
        print( "ScoreBoard: update()" );
    }
};

// Example minimal history
class History : public IHistory {
public:
    void clearHistory() override {
        print( "History: cleared" );
    }
};

// Example minimal GameState
class GameState : public IGameState {
public:
    bool gameRunning() const override {
        // Stub: always return true for demonstration
        return true;
    }
    int getCurrentAction() const override {
        return m_currentAction;
    }
    void setCurrentAction( int action ) override {
        m_currentAction = action;
    }
    std::map<int, int> getPlayer1SetHistory() const override {
        return m_p1History;
    }
    std::map<int, int> getPlayer2SetHistory() const override {
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
        print( "GameObject: loopGame()" );
    }
    std::shared_ptr<IScoreBoard> getScoreBoard() override {
        return m_scoreboard;
    }
    std::shared_ptr<IHistory> getHistory() override {
        return m_history;
    }
    void resetMatch() override {
        print( "GameObject: resetMatch()" );
    }
    void undo() override {
        print( "GameObject: undo()" );
    }
    void playerScore( int whichPlayer ) override {
        print( "GameObject: playerScore(" + std::to_string( whichPlayer ) + ")" );
    }

private:
    std::shared_ptr<IScoreBoard> m_scoreboard;
    std::shared_ptr<IHistory>    m_history;
};

// Example minimal Reset
class Reset : public IReset {
    // If you have logic for Reset, put it here
};

// Example minimal Inputs
class Inputs : public IInputs {
public:
    int read_mcp23017_value() override {
        // Stub input read for demonstration
        return 42;
    }
};

/*==========================================================
 *  Global signal variable to mimic the code's usage
 *==========================================================*/
static volatile std::sig_atomic_t gSignalStatus = 0;

/*==========================================================
 *  The Refactored run_remote_listener Function
 *
 *  Now uses only interface pointers and does not directly
 *  construct or call concrete implementations in-line.
 *==========================================================*/
void run_remote_listener( IGameObject* gameObject, IGameState* gameState, IReset* /*reset*/, IInputs* inputs ) {
    // Wrap gameState in a shared_ptr if needed by the constructor
    // (depends on how you manage lifetimes)
    std::shared_ptr<IGameState> spGameState(
        dynamic_cast<IGameState*>( gameState ) ? gameState : nullptr
    );

    // Create an IRemoteLocker via a concrete class (behind the scenes)
    std::unique_ptr<IRemoteLocker> remoteLocker = std::make_unique<RemoteLocker>( spGameState );
    print( "entered run remote listener method..." );
    int loop_count = 0;
    int selection = 0;
    print( "calling game object loop game..." );
    gameObject->loopGame();
    std::this_thread::sleep_for( std::chrono::seconds( 1 ));

    
    auto scoreboard = gameObject->getScoreBoard();        // Program to the IScoreBoard interface
    scoreboard->setLittleDrawerFont( "fonts/8x13B.bdf" );

    std::signal( SIGINT, IGameObject::_signalHandler );

    print ( " constructing remote pairing screen... " ); // Program to the IRemotePairingScreen interface
    std::unique_ptr<IRemotePairingScreen> remotePairingScreen = std::make_unique<RemotePairingScreen>( scoreboard );

    print( "constructing pairing blinker..." );
    std::shared_ptr<IPairingBlinker> pairingBlinker = std::make_shared<PairingBlinker>( scoreboard );

    print( "constructing input with timer..." );
    std::unique_ptr<IInputWithTimer> inputWithTimer =
        std::make_unique<InputWithTimer>( pairingBlinker, std::shared_ptr<IInputs>( inputs, []( IInputs* ){} ));
    print( "finished constructing input with timer..." );

    bool is_on_pi = scoreboard->onRaspberryPi();
    print( "is_on_pi: " + std::to_string( is_on_pi ));

    while ( gameState->gameRunning() && gSignalStatus != SIGINT ) { /*/// Begin Game Loop ///*/
        std::this_thread::sleep_for( std::chrono::seconds( SCORE_DELAY ));

        // if remote pairing, write the words.  if not, snap out of the loop
        while ( remotePairingScreen->inPairingMode() && /* is_on_pi && */ pairingBlinker->awake() ) {
            print( "inside remote pairing screen.  before starting input timer..." );

            if ( REMOTE_INPUT == 1 ) {
                print( "calling inputWithTimer->getInput()..." );
                selection = inputWithTimer->getInput();
            } else {
                std::cin >> selection;
                print( "*** inside remote listener getting remote selection ***" );
                print( "selection: " + std::to_string( selection ));
            }

            if ( selection == GREEN_REMOTE_GREEN_SCORE ) {
                remotePairingScreen->greenPlayerPressed();
                pairingBlinker->setGreenPlayerPaired( true );
            }
            else if ( selection == RED_REMOTE_RED_SCORE ) {
                remotePairingScreen->redPlayerPressed();
                pairingBlinker->setRedPlayerPaired( true );
            }
            else {
                std::cout << "*** Invalid selection during remote pairing. ***\n";
                GameTimer::gameDelay( 1000 );
            }
        }

        if ( !pairingBlinker->awake() ) {
            print( "pairing blinker is not awake, stopping it... " );
            pairingBlinker->stop();
            print( "pairing blinker stopped.  now putting in sleep mode..." );
            gameState->setCurrentAction( SLEEP_MODE );
        }

        if ( gameState->getCurrentAction() == SLEEP_MODE ) {
            print( "current action is SLEEP_MODE" );
            // Program to the IScoreboardBlinker interface
            std::unique_ptr<IScoreboardBlinker> blinker = std::make_unique<ScoreboardBlinker>( scoreboard );

            std::unique_ptr<IInputWithTimer> inputWithTimer2 =
                std::make_unique<InputWithTimer>(
                    std::shared_ptr<IPairingBlinker>(), // no pairing blinker needed here
                    std::shared_ptr<IInputs>( inputs, []( IInputs* ){} )
                );
            int selection = inputWithTimer2->getInput();

            gameState->setCurrentAction( AFTER_SLEEP_MODE );
            std::cout << "time slept: " << inputWithTimer2->getTimeSlept() << std::endl;

            if ( selection == GREEN_REMOTE_GREEN_SCORE ||
                 selection == GREEN_REMOTE_RED_SCORE   ||
                 ( inputWithTimer2->getTimeSlept() > MAX_SLEEP * 1000 ) )
            {
                print( "reset match." );
                gameObject->resetMatch();
                print( "done resetting match." );
                if ( inputWithTimer2->getTimeSlept() > MAX_SLEEP * 1000 ) {
                    print( "time slept: " + std::to_string( inputWithTimer2->getTimeSlept() / 1000 ) + " seconds." );
                    print( "clearing History because max sleep time has been reached or exceeded." );
                    gameObject->getHistory()->clearHistory();
                    print( "done clearing history because max sleep time has been reached or exceeded." );
                }
                continue;
            }
            print( "setting game state current action to after sleep mode" );
            gameState->setCurrentAction( AFTER_SLEEP_MODE );
            print( "*** Going into last Match! ***" );
            print( "clearing screen..." );
            scoreboard->clearScreen();
            print( "cleared screen." );
            scoreboard->update();
            print( "updated scoreboard." );
        }
        else {
            if ( REMOTE_INPUT == 0 ) {
                std::cin >> selection;
                print( "selection: " + std::to_string( selection ));
            } else {
                bool done = false;
                while ( !done ) {
                    selection = inputs->read_mcp23017_value();
                    std::cout << "read selection from inputs: " << selection << std::endl;
                    if ( selection == GREEN_REMOTE_GREEN_SCORE ||
                         selection == GREEN_REMOTE_RED_SCORE   ||
                         selection == RED_REMOTE_GREEN_SCORE   ||
                         selection == RED_REMOTE_RED_SCORE     ||
                         selection == GREEN_REMOTE_UNDO        ||
                         selection == RED_REMOTE_UNDO )
                    {
                        std::cout << "selection: " << selection << " triggered the done flag, exiting while loop..." << std::endl;
                        done = true;
                    } else {
                        std::cout << "sleeping 250ms..." << std::endl;
                        GameTimer::gameDelay( 250 );
                    }
                }
            }
        }

        int serve_flag = remoteLocker->playerNotServing( selection );
        print( "*** serve_flag: " + std::to_string( serve_flag ) + " ***" );
        if ( serve_flag ) {
            print( "*** Warning: player not serving! ***" );
            continue;
        }

        print( "setting player button to selection: " + std::to_string( selection ) + " before calling loopGame()..." );

        if ( selection == GREEN_REMOTE_GREEN_SCORE ||
             selection == GREEN_REMOTE_RED_SCORE   ||
             selection == RED_REMOTE_GREEN_SCORE   ||
             selection == RED_REMOTE_RED_SCORE )
        {
            if ( selection == GREEN_REMOTE_GREEN_SCORE || selection == RED_REMOTE_GREEN_SCORE ) {
                print( "*** \n\n\nGreen player scored ***\n\n\n" );
                selection = 1; // represent GREEN
            } else if ( selection == GREEN_REMOTE_RED_SCORE || selection == RED_REMOTE_RED_SCORE ) {
                print( "\n\n\n*** Red player scored ***\n\n\n" );
                selection = 2; // represent RED
            }
            gameObject->playerScore( selection );
        }
        else if ( selection == GREEN_REMOTE_UNDO || selection == RED_REMOTE_UNDO ) {
            print( "\n\n\n*** Undo ***\n\n\n" );
            gameObject->undo();
        }
        else {
            std::cout << "\n\n\n*** Invalid selection ***\n\n\n" << std::endl;
            std::this_thread::sleep_for( std::chrono::seconds( SCORE_DELAY ));
            continue;
        }

        std::this_thread::sleep_for( std::chrono::seconds( SCORE_DELAY ));
        gameObject->loopGame();  // handle the player score flag
        loop_count++;

        std::map<int, int> _player1_set_history = gameState->getPlayer1SetHistory();
        std::map<int, int> _player2_set_history = gameState->getPlayer2SetHistory();
    } ///////// End Game Loop /////////
}


int main() {
    // Create concrete instances of your objects
    GameObject myGameObject;
    GameState myGameState;
    Reset myReset;
    Inputs myInputs;

    // Call the refactored function
    run_remote_listener(&myGameObject, &myGameState, &myReset, &myInputs);

    return 0;
}

```

# Your Task
Describe the program in markdown using specific language that will help another AI program reconstruct the given program in as high fidelity as possible.
