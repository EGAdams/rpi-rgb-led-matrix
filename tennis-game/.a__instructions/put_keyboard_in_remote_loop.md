In the following C++ code, the system listens on its GPIO pins for a remote that is connected to the system.  I want the system to listen to keyboard inputs as well.  Please modify the following code to listen for keyboard ( cin ) and remote inputs.
```cpp
void run_remote_listener( GameObject* gameObject, GameState* gameState, Reset* reset, Inputs* inputs ) {
    print( "entered run remote listener method..." );
    int loop_count = 0;
    int selection = 0;
    print( "calling game object loop game..." );
    gameObject->loopGame();
    sleep( 1 );
    gameObject->getScoreBoard()->setLittleDrawerFont( "fonts/8x13B.bdf" );
    std::signal( SIGINT, GameObject::_signalHandler );
    print (" constructing remote pairing screen from inside run remote listener method... " );
    RemotePairingScreen remotePairingScreen( gameObject->getScoreBoard() );
    print( "constructing pairing blinker from run manual game" );
    PairingBlinker pairingBlinker( gameObject->getScoreBoard() );  // Use PairingBlinker
    print( "constructing input with timer from run manual game" );
    InputWithTimer inputWithTimer( &pairingBlinker, inputs );  // Pass PairingBlinker
    print( "finished constructing input with timer from run manual game" );
    bool is_on_pi = gameObject->getScoreBoard()->onRaspberryPi();
    print( "is_on_pi: " + std::to_string( is_on_pi ) );
    while ( gameState->gameRunning() && GameObject::gSignalStatus != SIGINT ) { /*/// Begin Game Loop ///*/
        print( "entered while loop from run manual game" );
        sleep( SCORE_DELAY );
        // if remote pairing, write the words.  if not, snap out of the loop
        while ( remotePairingScreen.inPairingMode() && is_on_pi && pairingBlinker.awake() ) { // 090724
            print( "inside remote pairing screen from run manual game.  before starting input timer..." );
            selection = inputWithTimer.getInput();
            print( "selection: " << selection );
            if ( selection == 7 ) {
                remotePairingScreen.greenPlayerPressed();
                pairingBlinker.setGreenPlayerPaired( true );  // Notify blinker that Green player is paired
            }
            else if ( selection == 11 ) {
                remotePairingScreen.redPlayerPressed();
                pairingBlinker.setRedPlayerPaired( true );  // Notify blinker that Red player is paired
            }
            else {
                std::cout << "*** Invalid selection during remote pairing. ***\n";
                GameTimer::gameDelay( 1000 );
            }
        }

        print( "put in sleep mode if the pairing blinker is not awake. " );
        if ( !pairingBlinker.awake() ) {
            print( "pairing blinker is not awake, stopping it... " )
                pairingBlinker.stop();
            print( "pairing blinker stopped.  now putting in sleep mode..." );
            gameState->setCurrentAction( SLEEP_MODE );
        }

        if ( gameState->getCurrentAction() == SLEEP_MODE ) {
            ScoreboardBlinker blinker( gameObject->getScoreBoard() );
            InputWithTimer inputWithTimer( &blinker, inputs );
            int selection = inputWithTimer.getInput();
            gameState->setCurrentAction( AFTER_SLEEP_MODE ); // stop sleep mode
            std::cout << "time slept: " << inputWithTimer.getTimeSlept() << std::endl;
            if ( selection == 7 ||
                  selection == 11 ||
                 ( inputWithTimer.getTimeSlept() > MAX_SLEEP * 1000 ) ) { // and sleep time expired...
                print( "reset match." );
                gameObject->resetMatch();
                print( "done resetting match." );
                if ( inputWithTimer.getTimeSlept() > MAX_SLEEP * 1000 ) {
                    print( "time slept: " << inputWithTimer.getTimeSlept() / 1000 << " seconds." << std::endl );
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
            gameObject->getScoreBoard()->clearScreen();
            print( "cleared screen." );
            gameObject->getScoreBoard()->update();
            print( "updated scoreboard." );
        } else {
            selection = inputs->read_mcp23017_value();
            print( "selection: " << selection );
        }

        if ( selection == 7 || selection == 11 ) {
            if ( selection == 7 ) {
                selection = 1;
            } else if ( selection == 11 ) {
                selection = 2;
            }
            gameObject->playerScore( selection );  // flip the player score flag
            sleep( SCORE_DELAY );
        } else {
            std::cout << "\n\n\n\n\n\n\n*** Invalid selection ***\n" << std::endl;
            sleep( SCORE_DELAY );
            continue;
        }
        gameObject->loopGame();  // handle the player score flag
        loop_count++;
        std::map<int, int> _player1_set_history = gameState->getPlayer1SetHistory();
        std::map<int, int> _player2_set_history = gameState->getPlayer2SetHistory();
    } ///////// End Game Loop /////////
}
```
Also, please remove the excessive print statements 