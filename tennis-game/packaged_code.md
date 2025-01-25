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
RemoteInputWithTimer::RemoteInputWithTimer( Blinker* blinker, Inputs* inputs, unsigned long timeout_ms )
    : IInputWithTimer( blinker, timeout_ms ), _inputs( inputs ) { // Call the correct base class constructor and initialize _inputs
    std::cout << "RemoteInputWithTimer constructor called" << std::endl; }

RemoteInputWithTimer::~RemoteInputWithTimer() {}

int RemoteInputWithTimer::getInput() {
    using namespace std::chrono;
    auto startTime = steady_clock::now();
    unsigned long elapsedTimeMs = 0;
    unsigned long sleep_start = GameTimer::gameMillis(); // Mark start time with game timer
    int selection;
    bool done = false;
    print( "starting blinker from within RemoteInputWithTimer..." );
    _blinker->start();
    print( "getting input from within RemoteInputWithTimer..." );
    if ( REMOTE_INPUT == 1 ) {  // 122224
        /*// if the selection is never one of the valid remote inputs, then we will never exit the while loop! // 011925
         * there is no timer here, the agent says.  in the future we will take out this comment and ask a new employee a question about this dillemma.  we will ask them to explain why this is a bad design because it locks up the system. meaning,  so how do we fix this?
         * the timer that would allow us to break out of this while loop is not in the scope of this while loop! we need either to make a timer in here, or pass in a timer object to this function.  */
         // TODO: add an outside timer that will break out of this while loop.
        while ( !done ) {
            auto now = steady_clock::now();
            elapsedTimeMs = duration_cast< milliseconds >( now - startTime ).count();

            if ( elapsedTimeMs >= _timeout_ms ) {
                std::cout << "Keyboard input timed out after " << _timeout_ms / 1000 << " seconds." << std::endl;
                return _timeout_ms; // <--<< this is where we break out of the loop!  the other input with timer is missing this.
            }
            print( "*** reading selection from inputs... ***" ); // 122224
            selection = _inputs->read_mcp23017_value();  // this actually does have a while. // 011925
            std::cout << "read selection from inputs: " << selection << std::endl; // but it
            if ( selection == GREEN_REMOTE_GREEN_SCORE ||                         // isnt as bad as this
                 selection == GREEN_REMOTE_RED_SCORE ||                         // one because it always returns a value
                 selection == RED_REMOTE_GREEN_SCORE ||                         // 011925
                 selection == RED_REMOTE_RED_SCORE ) {
                std::cout << "selection: " << selection << " triggered the done flag, exiting while loop..." << std::endl;
                done = true;
            }
            else {
                // delay 250ms
                std::cout << "sleeping 250ms..." << std::endl; // so we end up reading this over and over // 011925
                GameTimer::gameDelay( 250 );
            }
        }
    }
    else if ( REMOTE_INPUT == 0 ) {  // menu mode // 122224
        std::cout << "Enter your selection: ";
        std::cin >> selection;
        print( "made seleciton in RemoteInputWithTimer::getInput()... " );
        print( "selection: " << selection );

    }
    else {
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
    void start() override {
        // No operation
        print( "Blank Blinker started." );
    }

    void stop() override {
        // No operation
        print( "Blank Blinker stopped." );
    }
}
```

Here is the output:
```bash
[22:36:26] [tennis-game.cpp] [: 1050] [main()] creating reset object...
[22:36:26] [tennis-game.cpp] [: 1058] [main()] running game from remote inputs...
[22:36:26] [tennis-game.cpp] [: 871] [run_remote_listener()] entered run remote listener method...
[22:36:26] [tennis-game.cpp] [: 874] [run_remote_listener()] calling game object loop game...
[22:36:27] [ScoreBoard/ScoreBoard.cpp] [: 546] [setLittleDrawerFont()] loading little number font: fonts/8x13B.bdf
[22:36:28] [ScoreBoard/ScoreBoard.cpp] [: 550] [setLittleDrawerFont()] little number font loaded
[22:36:28] [ScoreBoard/ScoreBoard.cpp] [: 551] [setLittleDrawerFont()] set little number font disabled on october massacre.
[22:36:28] [tennis-game.cpp] [: 885] [run_remote_listener()]  constructing blinkers...
PairingBlinker constructing...
[22:36:28] [tennis-game.cpp] [: 896] [run_remote_listener()] is_on_pi: 1
[22:36:28] [RemoteInputWithTimer/RemoteInputWithTimer.cpp] [: 13] [RemoteInputWithTimer()] RemoteInputWithTimer constructor called, checking blinker...
[22:36:28] [RemoteInputWithTimer/RemoteInputWithTimer.cpp] [: 18] [RemoteInputWithTimer()] *** Blinker is not null, continuing... ***
RemoteInputWithTimer constructor called
[22:36:28] [RemoteInputWithTimer/RemoteInputWithTimer.cpp] [: 13] [RemoteInputWithTimer()] RemoteInputWithTimer constructor called, checking blinker...
[22:36:28] [RemoteInputWithTimer/RemoteInputWithTimer.cpp] [: 18] [RemoteInputWithTimer()] *** Blinker is not null, continuing... ***
RemoteInputWithTimer constructor called
[22:36:28] [RemoteInputWithTimer/RemoteInputWithTimer.cpp] [: 13] [RemoteInputWithTimer()] RemoteInputWithTimer constructor called, checking blinker...
[22:36:28] [RemoteInputWithTimer/RemoteInputWithTimer.cpp] [: 18] [RemoteInputWithTimer()] *** Blinker is not null, continuing... ***
RemoteInputWithTimer constructor called
[22:36:28] [RemoteGameInput/RemoteGameInput.cpp] [: 4] [RemoteGameInput()] constructing remote game input...
[22:36:28] [tennis-game.cpp] [: 913] [run_remote_listener()] inside remote pairing screen from run manual game.  before starting input timer...
[22:36:28] [RemoteInputWithTimer/RemoteInputWithTimer.cpp] [: 31] [getInput()] starting blinker from within RemoteInputWithTimer...
[22:36:28] [RemoteInputWithTimer/RemoteInputWithTimer.cpp] [: 36] [getInput()] *** Blinker is not null, continuing... ***
Segmentation fault
dietpi@DietPi:~/rpi-rgb-led-matrix/tennis-game$
```

Please help me debug this Segmentation Fault.
