# Who you are
You are a world-class C++ Developer and seasoned user of the GoF Design Patterns
You always write modular, testable, and maintainable code.

# What we are building
We are focusing on building the beginning part of a tennis game. The reason why we are building this part first is because it is the most complex part of the game.  We want to get the logic right before we start adding anything else.  We need to focus on thread safety to prevent segmentation faults, memory leaks, and other issues that can arise from concurrent access to shared resources.

## Beginning Game States
Here are the states of the game that we want to focus on:
1. Pairing Mode
2. Sleep Mode
3. Regular Game Play Before any Scores
4. Regular Game Play After a Score

## Input types
* Remote Input - This is the input from the remote that is sent to the system.
* Keyboard Input - This is the input from the keyboard that is sent to the system.
# C++ Source Code
```cpp
void run_remote_listener( GameObject* gameObject, GameState* gameStatearg, Reset* reset ) {
    int KEYBOARD_TIMEOUT = 120000;
    GameState* gameState = gameStatearg;
    RemoteLocker*       remoteLocker = new RemoteLocker( gameState );
    IInputWithTimer*    pairingInputWithTimer;
    IInputWithTimer*    noBlinkInputWithTimer;
    IInputWithTimer*    sleepingInputWithTimer;
    IGameInput*         gameInput;

    Inputs* inputs      = new Inputs( gameObject->getPlayer1(), gameObject->getPlayer2(), gameObject->getPinInterface(), gameState );
    bool keyboard_off   = true; // set this to true to use the remotes.
    bool no_score       = true;
    int loop_count      = 0;
    int selection       = 0;
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
    // checking pairing blinker for null state
    if ( pairingBlinker == nullptr ) {
        print( "*** ERROR: pairingBlinker is null! ***" );
    } else {
        print( "*** pairingBlinker is not null, continuing... ***" );
    }
    print( "is_on_pi: " + std::to_string( is_on_pi ));
    if ( is_on_pi && keyboard_off ) {            // to use the keyboard, switch keyboard_off to false
        pairingInputWithTimer       = new RemoteInputWithTimer( pairingBlinker, inputs, pairing_timer   );
        noBlinkInputWithTimer       = new RemoteInputWithTimer( blankBlinker, inputs,   no_blink_timer  );
        sleepingInputWithTimer      = new RemoteInputWithTimer( sleepingBlinker, inputs, sleeping_timer );
        gameInput                   = new RemoteGameInput(      inputs                                  );
    } else {
        pairingInputWithTimer       = new KeyboardInputWithTimer( pairingBlinker, KEYBOARD_TIMEOUT      );
        noBlinkInputWithTimer       = new KeyboardInputWithTimer( blankBlinker, KEYBOARD_TIMEOUT        );
        sleepingInputWithTimer      = new KeyboardInputWithTimer( sleepingBlinker, KEYBOARD_TIMEOUT     );
        gameInput                   = new KeyboardGameInput(                                            );     
    }
    while ( gameState->gameRunning() && gSignalStatus != SIGINT ) { /*/// Begin Game Loop ///*/
        std::this_thread::sleep_for( std::chrono::seconds( SCORE_DELAY ));

        // if remote pairing, write the words.  if not, snap out of the loop
        print( "in pairing mode? " + std::to_string( remotePairingScreen->inPairingMode()));
        print( "pairingBlinker->awake(): " + std::to_string( pairingBlinker->awake()));
        while ( remotePairingScreen->inPairingMode() && pairingBlinker->awake()) {
            print( "inside remote pairing screen from run remote listener.  before starting input timer..." );
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

        if ( !pairingBlinker->awake()) {
            print( "pairing blinker is not awake, stopping it... " );
            pairingBlinker->stop();
            print( "pairing blinker stopped.  now putting in sleep mode..." );
            gameState->setCurrentAction( SLEEP_MODE );
        }

        if ( gameState->getCurrentAction() == SLEEP_MODE ) {
            sleepingInputWithTimer->getInput();                 // blocks here for so many seconds
            gameState->setCurrentAction( AFTER_SLEEP_MODE );
            print( "time slept: " << sleepingInputWithTimer->getTimeSlept());

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
        if ( no_score ) {   // if there is no score, we need to time the input for the Zero score timeout.
            selection = noBlinkInputWithTimer->getInput();
            print( "selection: " + std::to_string( selection ));

            /* CHECK FOR TIMEOUT */
            // if timed out, selection is the timeout value, not a valid game input.  the timeout value is no where near a valid game input.    
            if ( selection > 1000 ) {                       // this is a Zero score timeout
                print( "*** Zero ScoreTimeout!  going to sleep mode... ***" );
                gameState->setCurrentAction( SLEEP_MODE );  // set to sleep mode and go to the beginning
                continue;
            }
        }

        if ( no_score ) { 
            print( "setting no_score to false..." ); no_score = false; // no need for the zero score timer anymore
        } else {
            selection = gameInput->getInput();  // this is either cin << or a remote call, both block until input is made
        }                                       // there are no timers in regular game play at the time of this writing
                                                // January 25, 2025  mass deportation began.  it was very cold in FL last night.
        if ( selection == 0 ) {  // not sure if we need this here, but it won't hurt for now - 012525
            print( "\n\nselection: " + std::to_string( selection ) + " ***\n\n" );
            print( "*** Invalid selection! ***  continuing..." );
            continue;
        }

        int serve_flag = remoteLocker->playerNotServing( selection );
        print( "*** serve_flag: " + std::to_string( serve_flag ) + " ***" );
        if ( serve_flag ) {
            print( "*** Warning: player not serving! ***" );
            continue;  // do not want to kill the no score flag quite yet
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
            print( "------------" );
            print( "GREEN REMOTE: " );
            print( "   green remote green score: " <<  GREEN_REMOTE_GREEN_SCORE );
            print( "or green remote, red score: " << GREEN_REMOTE_RED_SCORE );
            print( " ------------ \n");
            print( "RED REMOTE: " );
            print( "or red remote, green score: " << RED_REMOTE_GREEN_SCORE );
            print( "or red remote, red score: " << RED_REMOTE_RED_SCORE );
            print( " ------------ \n");
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

int main() {
    print( "creating game state object..." );
    GameState* gameState = new GameState();
    print( "creating game object..." );
    ColorManager* colorManager = new ColorManager();
    bool isOnPi = is_on_raspberry_pi();
    print( "isOnPi: " << isOnPi );
    IDisplay* display = new ConsoleDisplay( colorManager );
    if ( isOnPi ) {
        print( "creating display object with matrix display..." );
    }
    else {
        print( "creating display object with console display..." );
        display = new ConsoleDisplay( colorManager );
    }
    GameObject* gameObject = new GameObject( gameState, display );
    print( "creating reset object..." );
    Reset* reset = new Reset( gameObject->getPlayer1(), gameObject->getPlayer2(), gameObject->getPinInterface(), gameState );
    run_remote_listener( gameObject, gameState, reset ); return 0;

    // Clean up memory
    delete gameObject;
    delete gameState;
    delete reset;
    delete display;
    delete colorManager;

    return 0;
}
```

```cpp
int RemoteInputWithTimer::getInput() {
    using namespace std::chrono;
    auto startTime = steady_clock::now();
    unsigned long elapsedTimeMs = 0;
    unsigned long sleep_start = GameTimer::gameMillis(); // Mark start time with game timer
    int selection;
    bool done = false;
    _blinker->start();
    if ( REMOTE_INPUT == 1 ) {  // 122224
        while ( !done ) {
            auto now = steady_clock::now();
            elapsedTimeMs = duration_cast< milliseconds >( now - startTime ).count();

            if ( elapsedTimeMs >= _timeout_ms ) {
                std::cout << "Keyboard input timed out after " << _timeout_ms / 1000 << " seconds." << std::endl;
                return _timeout_ms; // <--<< this is where we break out of the loop!  the other input with timer is missing this.
            }
            selection = _inputs->read_mcp23017_value();
            std::cout << "read selection from inputs: " << selection << std::endl; 
            if ( selection == GREEN_REMOTE_GREEN_SCORE ||                          
                 selection == GREEN_REMOTE_RED_SCORE   ||                          
                 selection == RED_REMOTE_GREEN_SCORE   ||                          
                 selection == RED_REMOTE_RED_SCORE ) {
                std::cout << "selection: " << selection << " triggered the done flag, exiting while loop..." << std::endl;
                done = true;
            } else {
                std::cout << "sleeping 250ms..." << std::endl;
                GameTimer::gameDelay( 250 );
            }
        }
    } else if ( REMOTE_INPUT == 0 ) {  // menu mode // 122224
        std::cout << "Enter your selection: ";
        std::cin >> selection;
    } else {
        std::cout << "*** ERROR: Invalid input mode in RemoteInputWithTimer Object getInput() method. ***\n";
        exit( 1 );
    }
    _blinker->stop();
    unsigned long sleep_end = GameTimer::gameMillis(); // Mark end time with game timer
    _time_slept = sleep_end - sleep_start;
    return selection;
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

```cpp
class BlankBlinker : public Blinker {
public:
    BlankBlinker() : _should_stop(false) { }
    ~BlankBlinker() { stop(); }

    void start() override {
        _should_stop = false;
        _thread = std::thread(&BlankBlinker::threadLoop, this);
    }

    void stop() override {
        _should_stop = true;
        if (_thread.joinable()) {
            _thread.join();
        }
    }

private:
    void threadLoop() {
        // Minimal thread function—just exits immediately
        return;
    }

    std::thread _thread;
    std::atomic<bool> _should_stop;
}
```

```cpp
#include "PairingBlinker.h"
#include <chrono>
#include <thread>
#include <iostream>

PairingBlinker::PairingBlinker( ScoreBoard* scoreBoard )
    : _scoreboard( scoreBoard ), _should_stop( false ), _green_player_paired( false ), _red_player_paired( false ) {
    std::cout << "PairingBlinker constructing..." << std::endl;
    if ( _scoreboard == nullptr ) { // check for null _scoreboard
        std::cerr << "Error: _scoreboard is null in PairingBlinker constructor." << std::endl;
        return;  // TODO: louder error and exit!
    }
}

PairingBlinker::~PairingBlinker() { stop(); }

const unsigned long PAIRING_TIMEOUT = 5000;
void PairingBlinker::blinkLoop() {
    bool toggle_on = true;  // Start with Green instructions
    print( "starting blink loop..." );
    unsigned long pairing_start_time = GameTimer::gameMillis();
    while ( !_should_stop ) {
        print( "in blink loop..." );
        unsigned long current_time       = GameTimer::gameMillis();
        print( "current time: " + std::to_string( current_time ));
        unsigned long elapsed_time       = current_time - pairing_start_time;
        print( "elapsed time: " + std::to_string( elapsed_time ));
        // Check if timeout exceeded
        if ( elapsed_time > PAIRING_TIMEOUT ) {
            print( "Timeout exceeded. Switch to blinking ball mode in the future..." );
            print( "timeout not exceeded, continue blinking..." );
        }

        if ( !_scoreboard ) {
            std::cerr << "*** ERROR: _scoreboard is null inside PairingBlinker::blinkLoop()! ***" << std::endl;
        } // return; // or break, just so we exit the blink loop safely

        _scoreboard->clearScreen();
        if ( _green_player_paired && _red_player_paired ) { 
            print( "both players seem to be paired, break..." );
            break;  // If both players are paired, stop blinking
        } else if ( !_green_player_paired && _red_player_paired ) { // If only the Red player is paired, show Green player instructions
            if ( toggle_on ) {
                showGreenInstructions();
                print( "showing green instructions..." );
            } else {
                print( "show neutral text only..." );
                showPlayerPressYourRemoteText();
            }
            print( "alternating between green on and green off..." );
            toggle_on = !toggle_on;  // Alternate led on and led off
        }
        if ( _green_player_paired && !_red_player_paired ) { // If only the Green player is paired, show Red player instructions
            _show_green = false;
            print( "showing red instructions inside blink loop..." );
            if ( toggle_on ) {
                showRedInstructions();
                print( "showing red instructions..." );
            }
            else {
                print( "show neutral text only..." );
                showPlayerPressYourRemoteText();
            }
            print( "alternating between red on and red off..." );
            toggle_on = !toggle_on;  // Alternate led on and led off
        } else if ( !_green_player_paired && !_red_player_paired ) { // If neither player is paired, show Green instructions
            if ( toggle_on ) {
                showGreenInstructions();
                print( "showing green instructions..." );
            }
            else {
                print( "show neutral text only..." );
                showPlayerPressYourRemoteText();
            }
            print( "alternating between green on and green off..." );
            toggle_on = !toggle_on;  // Alternate led on and led off
        }
        std::this_thread::sleep_for( std::chrono::seconds( 1 )); // Delay for 1 second between switching
    }
    print( "should stop flag must be true, exiting pairing blinker blink loop..." );
}

void PairingBlinker::showGreenInstructions() {
    Color red_color( 255, 0, 0 );
    Color green_color( 0, 255, 0 );
    Color yellow_color( 255, 255, 0 );
    int left_margin = 9;
    print( "clearing screen... " );
    _scoreboard->clearScreen();
    print( "setting drawer foreground color to green..." );
    _scoreboard->setDrawerForegroundColor( green_color );
    print( "drawing Green text..." );
    _scoreboard->drawNewText( "Green", left_margin + GREEN_OFFSET, 17 );
    print( "drawing Player text..." );
    _scoreboard->drawNewText( "Player", left_margin + 0, PLAYER_TOP );
    _scoreboard->setDrawerForegroundColor( yellow_color );
    _scoreboard->drawNewText( "Press", left_margin + PRESS_OFFSET, PRESS_TOP + MIDDLE_OFFSET );
    _scoreboard->drawNewText( "Your", left_margin + YOUR_OFFSET, YOUR_TOP + MIDDLE_OFFSET);
    _scoreboard->drawNewText( "Remote", left_margin + REMOTE_OFFSET, REMOTE_TOP + MIDDLE_OFFSET );
    _scoreboard->setDrawerForegroundColor( green_color );
    _scoreboard->drawNewText( "Green", left_margin + GREEN_OFFSET, GREEN_TOP );
    _scoreboard->drawNewText( "Button", left_margin + 0, BUTTON_TOP ); 
}

void PairingBlinker::showRedInstructions() {
    Color red_color( 255, 0, 0 );
    Color green_color( 0, 255, 0 );
    Color yellow_color( 255, 255, 0 );
    if ( _scoreboard->onRaspberryPi() ) {
        int left_margin = 9;
        _scoreboard->clearScreen();
        _scoreboard->setDrawerForegroundColor( red_color );
        _scoreboard->drawNewText( "Red", left_margin + RED_OFFSET, 17 );
        _scoreboard->drawNewText( "Player", left_margin + 0, PLAYER_TOP );
        _scoreboard->setDrawerForegroundColor( yellow_color );
        _scoreboard->drawNewText( "Press", left_margin + PRESS_OFFSET, PRESS_TOP + MIDDLE_OFFSET );
        _scoreboard->drawNewText( "Your", left_margin + YOUR_OFFSET, YOUR_TOP + MIDDLE_OFFSET );
        _scoreboard->drawNewText( "Remote", left_margin + REMOTE_OFFSET, REMOTE_TOP + MIDDLE_OFFSET );
        _scoreboard->setDrawerForegroundColor( red_color );
        _scoreboard->drawNewText( "Red", left_margin + RED_OFFSET, RED_TOP );
        _scoreboard->drawNewText( "Button", left_margin + 0, BUTTON_TOP );
    }
}

void PairingBlinker::showPlayerPressYourRemoteText() {
    Color red_color( 255, 0, 0 );
    Color green_color( 0, 255, 0 );
    Color yellow_color( 255, 255, 0 );
    int left_margin = 9;
    _scoreboard->clearScreen();
    if ( _show_green ) {
        _scoreboard->setDrawerForegroundColor( green_color );
        _scoreboard->drawNewText( "Player", left_margin + 0, PLAYER_TOP );
    } else {
        _scoreboard->setDrawerForegroundColor( red_color );
        _scoreboard->drawNewText( "Player", left_margin + 0, PLAYER_TOP );
    }
    _scoreboard->setDrawerForegroundColor( yellow_color );
    _scoreboard->drawNewText( "Press", left_margin + PRESS_OFFSET, PRESS_TOP + MIDDLE_OFFSET );
    _scoreboard->drawNewText( "Your", left_margin + YOUR_OFFSET, YOUR_TOP + MIDDLE_OFFSET );
    _scoreboard->drawNewText( "Remote", left_margin + REMOTE_OFFSET, REMOTE_TOP + MIDDLE_OFFSET );
    if ( _show_green ) {
        _scoreboard->setDrawerForegroundColor( green_color );
        _scoreboard->drawNewText( "Green", left_margin + GREEN_OFFSET, GREEN_TOP );
        _scoreboard->drawNewText( "Button", left_margin + 0, BUTTON_TOP );
    } else {
        _scoreboard->setDrawerForegroundColor( red_color );
        _scoreboard->drawNewText( "Red", left_margin + RED_OFFSET, RED_TOP );
        _scoreboard->drawNewText( "Button", left_margin + 0, BUTTON_TOP );
    }
}

void PairingBlinker::start() {
    // print( "starting blink thread..." );
    _should_stop = false;
    blink_thread = std::thread( &PairingBlinker::blinkLoop, this );
    // print( "blink thread started..." );
}

void PairingBlinker::stop() {
    _should_stop = true;
    if ( blink_thread.joinable() ) {
        blink_thread.join();
    }
}

void PairingBlinker::enable() {            _should_stop = false; }
void PairingBlinker::sleepModeOn() {       _sleep_mode  = true;  }
void PairingBlinker::sleepModeOff() {      _sleep_mode  = false; }
bool PairingBlinker::awake() {     return !_sleep_mode;          }
void PairingBlinker::setGreenPlayerPaired( bool paired ) { _green_player_paired = paired; }
void PairingBlinker::setRedPlayerPaired(   bool paired ) { _red_player_paired = paired;   }
```

```cpp
class IInputWithTimer {
protected:
    Blinker* _blinker;
    unsigned long _timeout_ms;  // Timeout duration in milliseconds
    unsigned long _time_slept;  // Time slept in milliseconds

    IInputWithTimer(Blinker* blinker, unsigned long timeout_ms)
        : _blinker(blinker), _timeout_ms(timeout_ms), _time_slept(0) {
            print( "IInputWithTimer constructor called" );
            // check if blinker is null
            if (blinker == nullptr) {
                print( "*** ERROR: blinker is null! ***" );
            } else {
                print( "*** blinker is not null, continuing... ***" );
            }
        }

public:
    virtual ~IInputWithTimer() = default; // Pure virtual methods to enforce 
    virtual int getInput() = 0;           // implementation in derived classes
    unsigned long getTimeSlept() const;
    void setTimeout(unsigned long timeout);
}
```

# My Humble Opinion
I think that we could use the strategy pattern here because the behavior is a little different depending on the state of the game.  Maybe there is such a thing as a State pattern that we could use here, but I'm not sure.  That's my 2 cents.   

# Your Task
Rewrite the run_remote_listener.cpp file using your profound knowledge of software design patterns to conform to the specifications.  If you need more Interfaces, just put them in your code block answer.  When you have come up with a solution, write all of the C++ code into on code block.  Don't worry about rewriting the Interfaces or any code that we already have, just rewrite the run_remote_listener.cpp.  Remember to pay close attention to thread safety.  If you have any questions or need to see code that I have not included, please let me know what I can do to help before you write any code. 