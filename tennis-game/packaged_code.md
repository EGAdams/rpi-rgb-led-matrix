The following C++ selection varialble is zero here: ```cpp selection = gameInput->getInput(); ```  In the following C++ code when the run_remote_listener() method is called, I press "6" and then "7" to get the system out of Pairing Mode.  Then in the regular game mode, I press "6" or "7" to trigger a player score.  Please analyze the following C++ code to figure out how this game works:
```cpp
void run_remote_listener( GameObject* gameObject, GameState* gameStatearg, Reset* reset ) {
    int KEYBOARD_TIMEOUT = 10000;
    GameState* gameState = gameStatearg;
    RemoteLocker*       remoteLocker = new RemoteLocker( gameState );
    IInputWithTimer*    pairingInputWithTimer;
    IInputWithTimer*    noBlinkInputWithTimer;
    IInputWithTimer*    sleepingInputWithTimer;
    IGameInput*         gameInput;

    Inputs*             inputs = new Inputs( gameObject->getPlayer1(), gameObject->getPlayer2(), gameObject->getPinInterface(), gameState );
    bool no_score = true;
    print( "entered run remote listener method..." );
    int loop_count = 0;
    int selection = 0;
    print( "calling game object loop game..." );
    gameObject->loopGame();
    std::this_thread::sleep_for( std::chrono::seconds( 1 ));
    

    // Program to the IScoreBoard interface
    auto scoreboard = gameObject->getScoreBoard();
    scoreboard->setLittleDrawerFont( "fonts/8x13B.bdf" );

    std::signal( SIGINT, GameObject::_signalHandler );

    print ( " constructing blinkers... " );
    RemotePairingScreen*    remotePairingScreen = new RemotePairingScreen( scoreboard );
    PairingBlinker*         pairingBlinker      = new PairingBlinker( scoreboard );
    BlankBlinker*           blankBlinker        = new BlankBlinker();
    ScoreboardBlinker*      sleepingBlinker     = new ScoreboardBlinker( scoreboard );

    unsigned long pairing_timer   = 4000;
    unsigned long no_blink_timer  = 4000;
    unsigned long sleeping_timer  = 4000;

    bool is_on_pi = scoreboard->onRaspberryPi();
    print( "is_on_pi: " + std::to_string( is_on_pi ));
    if ( is_on_pi ) {
        pairingInputWithTimer       = new RemoteInputWithTimer( pairingBlinker, inputs, pairing_timer   );
        noBlinkInputWithTimer       = new RemoteInputWithTimer( blankBlinker, inputs,   no_blink_timer  );
        sleepingInputWithTimer      = new RemoteInputWithTimer( sleepingBlinker, inputs, sleeping_timer );
        gameInput                   = new RemoteGameInput( inputs );
    } else {
        pairingInputWithTimer       = new KeyboardInputWithTimer( pairingBlinker, KEYBOARD_TIMEOUT      );
        noBlinkInputWithTimer       = new KeyboardInputWithTimer( blankBlinker, KEYBOARD_TIMEOUT        );
        sleepingInputWithTimer      = new KeyboardInputWithTimer( sleepingBlinker, KEYBOARD_TIMEOUT     );
        gameInput                   = new KeyboardGameInput(                         );     
    }
    while ( gameState->gameRunning() && gSignalStatus != SIGINT ) { /*/// Begin Game Loop ///*/
        std::this_thread::sleep_for( std::chrono::seconds( SCORE_DELAY ));

        // if remote pairing, write the words.  if not, snap out of the loop
        while ( remotePairingScreen->inPairingMode() && pairingBlinker->awake() ) {
            print( "inside remote pairing screen from run manual game.  before starting input timer..." );
            selection = pairingInputWithTimer->getInput();
            if ( selection == GREEN_REMOTE_GREEN_SCORE ) {
                remotePairingScreen->greenPlayerPressed();
                pairingBlinker->setGreenPlayerPaired( true );
            } else if ( selection == RED_REMOTE_RED_SCORE ) {
                remotePairingScreen->redPlayerPressed();
                pairingBlinker->setRedPlayerPaired( true );
            } else {
                print( "*** Invalid selection during pairing. *** 012525\n\n\n" );
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
            sleepingInputWithTimer->getInput();                 // blocks here for so many seconds
            gameState->setCurrentAction( AFTER_SLEEP_MODE );
            print( "time slept: " << sleepingInputWithTimer->getTimeSlept() );

            if ( selection == GREEN_REMOTE_GREEN_SCORE || selection == GREEN_REMOTE_RED_SCORE ) {
                print( "reset match." );
                gameObject->resetMatch();
                print( "done resetting match." );
                if ( sleepingInputWithTimer->getTimeSlept() > MAX_SLEEP * 1000 ) {
                    print( "time slept: " + std::to_string( sleepingInputWithTimer->getTimeSlept() / 1000 ) + " seconds." );
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
        }                       // else NOT in SLEEP_MODE
        
        // we still need to set the time for a no score mode
        if ( no_score ) {   // monitor the input if no score has been made yet.
                selection = noBlinkInputWithTimer->getInput();
                print( "selection: " + std::to_string( selection ));

            if ( selection > 1000 ) {                       // this is a Zero score timeout
                print( "*** Zero ScoreTimeout!  going to sleep mode... ***" );
                gameState->setCurrentAction( SLEEP_MODE );  // set to sleep mode and go to the beginning
                continue;
            }
        }

        int serve_flag = remoteLocker->playerNotServing( selection );
        print( "*** serve_flag: " + std::to_string( serve_flag ) + " ***" );
        if ( serve_flag ) {
            print( "*** Warning: player not serving! ***" );
            continue;
        }
        
        selection = gameInput->getInput();
        if ( selection == 0 ) {
            print( "\n\nselection: " + std::to_string( selection ) + " ***\n\n" );
            print( "*** Invalid selection! ***  continuing..." );
            continue;
        }

        print( "setting player button to selection: " + std::to_string( selection ) + " before calling loopGame()..." );
        if ( selection == GREEN_REMOTE_GREEN_SCORE ||
             selection == GREEN_REMOTE_RED_SCORE   ||
             selection == RED_REMOTE_GREEN_SCORE   ||
             selection == RED_REMOTE_RED_SCORE ) {
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
            print( "\n\n\n*** Invalid selection ***\n\n\n" );
            std::this_thread::sleep_for( std::chrono::seconds( SCORE_DELAY ));
            continue;
        }

        std::this_thread::sleep_for( std::chrono::seconds( SCORE_DELAY ));
        gameObject->loopGame();  // handle the player score flag
        loop_count++;

        // Just to mimic the original code's retrieval of set history
        std::map<int, int> _player1_set_history = gameState->getPlayer1SetHistory();
        std::map<int, int> _player2_set_history = gameState->getPlayer2SetHistory();
    } ///////// End Game Loop /////////
}
```
```cpp

RemotePairingScreen::RemotePairingScreen( ScoreBoard* scoreboard ) 
    : _green_player_paired( false ), _red_player_paired( false ), _scoreboard( scoreboard ) {
void RemotePairingScreen::draw() {
    if (!_green_player_paired) {
        if ( _scoreboard->onRaspberryPi()) {
```

```cpp

#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include "../Blinker/Blinker.h"
#include "../KeyboardInputWithTimer/KeyboardInputWithTimer.h"
#include "KeyboardInputWithTimer.h"
#include "../TennisConstants/TennisConstants.h"

KeyboardInputWithTimer::KeyboardInputWithTimer( Blinker* blinker, unsigned long timeout_ms )
    : IInputWithTimer( blinker, timeout_ms ), _elapsedTimeMs( 0 ) {
}  // initialize elapsedTimeMs to 0

KeyboardInputWithTimer::~KeyboardInputWithTimer() {
    print( "\n\n*** keyboard destructing without restoring the keyboard. *** \n\n" );
    // _restoreTerminal( oldt, old_flags );
}

bool KeyboardInputWithTimer::validateInput( int selection ) const {
    return selection == 6 || selection == 7 || selection == 9;
}

int KeyboardInputWithTimer::_configureTerminal( struct termios& oldt ) {
    struct termios newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO ); // Disable canonical mode and echo
    tcsetattr( 0, TCSANOW, &newt );

    int old_flags = fcntl( 0, F_GETFL, 0 );
    fcntl( 0, F_SETFL, old_flags | O_NONBLOCK );
    return old_flags;
}

void KeyboardInputWithTimer::_restoreTerminal( const struct termios& oldt, int old_flags ) {
    tcsetattr( 0, TCSANOW, &oldt );
    fcntl( 0, F_SETFL, old_flags );
}


int KeyboardInputWithTimer::getInput() {
    using namespace std::chrono;

    struct termios oldt;
    tcgetattr( 0, &oldt );
    int old_flags = _configureTerminal( oldt );

    _startTime = steady_clock::now();
    _elapsedTimeMs = 0;
    int selection = 0;
    std::string inputBuffer;

    try {
        while ( true ) {
            auto now = steady_clock::now();
            _elapsedTimeMs = duration_cast< milliseconds >( now - _startTime ).count();

            if ( _elapsedTimeMs >= _timeout_ms ) {
                print( "\n\n\n\n*** Timeout ***" );
                print( "Keyboard input timed out after " << _timeout_ms / 1000 << " seconds. 012525" );
                print( "\n\n\n\n" );
                return _elapsedTimeMs;
            }

            char ch;
            while ( read( 0, &ch, 1 ) > 0 ) {
                if ( ch == '\n' ) {
                    try {
                        selection = std::stoi( inputBuffer );
                        if ( validateInput( selection ) ) {
                            print( "Valid input received: " << selection );
                            return selection;
                        }
                        else {
                            print( "Invalid selection. Please enter 6, 7, or 9." );
                            inputBuffer.clear();
                        }
                    }
                    catch ( const std::invalid_argument& ) {
                        print( "Invalid input. Please enter a number." );
                        inputBuffer.clear();
                    }
                }
                else {
                    inputBuffer += ch;
                }
            }
            std::this_thread::sleep_for( milliseconds( 50 ) ); // Reduce CPU usage
        }
    }
    catch ( ... ) {
        _restoreTerminal( oldt, old_flags );
        throw;
    }
    _restoreTerminal( oldt, old_flags );
    return -1;
}
```

```cpp
class Blinker {
public:
    virtual ~Blinker() = default;
    
    virtual void start() = 0;
    virtual void stop() = 0;
}
```

The Standard Input is altered in the KeyboardWithInputTimer class, but I don't thik that it is ever set back to the original state.  Please rewrite the KeyboardWithInputTimer class so that this code does what it is supposed to do and uses std::cin to get the input after exiting the Pairing Mode.

