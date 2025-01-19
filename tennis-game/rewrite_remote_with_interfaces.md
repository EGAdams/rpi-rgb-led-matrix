# Persona
You are a world-class C++ Developer seasoned in using GoF Design Patterns and modular code writing.

# Background
I was always told to "program to the Interface, not the implementation".  The following C++ code violates that principle and is causing all sorts of problems.

# Your Task
Will you please rewrite the code with as many C++ Interfaces that you can.  I do not want to violate that principle at all, so we need to refactor the C++ code below.  Just give your output in one C++ code block so that I can paste the output into the same place where the C++ method was before and have it work as you have redesigned it to.  I'll break it up into different files later.

Here's the C++ Source Code:
```cpp
void run_remote_listener( GameObject* gameObject, GameState* gameState, Reset* reset, Inputs* inputs ) {
    RemoteLocker remoteLocker( gameState );
    print( "entered run remote listener method..." );
    int loop_count = 0;
    int selection = 0;
    print( "calling game object loop game..." );
    gameObject->loopGame();
    sleep( 1 );
    gameObject->getScoreBoard()->setLittleDrawerFont( "fonts/8x13B.bdf" );
    std::signal( SIGINT, GameObject::_signalHandler );
    print (" constructing remote pairing screen from inside run remote listener method... " );
    RemotePairingScreen remotePairingScreen( gameObject->getScoreBoard());
    print( "constructing pairing blinker from run manual game" );
    PairingBlinker pairingBlinker( gameObject->getScoreBoard());  // Use PairingBlinker
    print( "constructing input with timer from run manual game" );
    InputWithTimer inputWithTimer( &pairingBlinker, inputs );  // Pass PairingBlinker
    print( "finished constructing input with timer from run manual game" );
    bool is_on_pi = gameObject->getScoreBoard()->onRaspberryPi();
    print( "is_on_pi: " + std::to_string( is_on_pi ));
    while ( gameState->gameRunning() && GameObject::gSignalStatus != SIGINT ) { /*/// Begin Game Loop ///*/
        // print( "*** entered /*/// Begin Game Loop ///*/ ***" );
        // print( "entered while loop from run manual game" );
        sleep( SCORE_DELAY );
        // if remote pairing, write the words.  if not, snap out of the loop
        while ( remotePairingScreen.inPairingMode() && /* is_on_pi  && */ pairingBlinker.awake()) { // 090724
            print( "inside remote pairing screen from run manual game.  before starting input timer..." );
            
            if ( REMOTE_INPUT == 1 ) {
                selection = inputWithTimer.getInput();
            } else {
                std::cin >> selection;
                print( "*** inside remote listener getting remote selection ***" );
                print( "selection: " << selection );

            }
            if ( selection == GREEN_REMOTE_GREEN_SCORE ) {
                remotePairingScreen.greenPlayerPressed();
                pairingBlinker.setGreenPlayerPaired( true );  // Notify blinker that Green player is paired
            }
            else if ( selection == RED_REMOTE_RED_SCORE ) {
                remotePairingScreen.redPlayerPressed();
                pairingBlinker.setRedPlayerPaired( true );  // Notify blinker that Red player is paired
            }
            else {
                std::cout << "*** Invalid selection during remote pairing. ***\n";
                GameTimer::gameDelay( 1000 );
            }
        }

        // print( "put in sleep mode if the pairing blinker is not awake. " );
        if ( !pairingBlinker.awake()) {
            print( "pairing blinker is not awake, stopping it... " );
                pairingBlinker.stop();
            print( "pairing blinker stopped.  now putting in sleep mode..." );
            gameState->setCurrentAction( SLEEP_MODE );
        }

        if ( gameState->getCurrentAction() == SLEEP_MODE ) {
            ScoreboardBlinker blinker( gameObject->getScoreBoard());
            InputWithTimer inputWithTimer( &blinker, inputs );
            int selection = inputWithTimer.getInput();
            gameState->setCurrentAction( AFTER_SLEEP_MODE ); // stop sleep mode
            std::cout << "time slept: " << inputWithTimer.getTimeSlept() << std::endl;
            if ( selection == GREEN_REMOTE_GREEN_SCORE ||
                  selection == GREEN_REMOTE_RED_SCORE ||
                 ( inputWithTimer.getTimeSlept() > MAX_SLEEP * 1000 )) { // and sleep time expired...
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
            if ( REMOTE_INPUT == 0 ) {
                std::cin >> selection;
                // print( "*** selection made in /*/// Begin Game Loop ///*/ ***" );
                print( "selection: " << selection );
            } else {
                bool done = false;
                while ( !done ) {                           // remote mode
                    selection = inputs->read_mcp23017_value();
                    std::cout << "read selection from inputs: " << selection << std::endl;
                    if ( selection == GREEN_REMOTE_GREEN_SCORE  || 
                        selection == GREEN_REMOTE_RED_SCORE     ||
                        selection == RED_REMOTE_GREEN_SCORE     ||
                        selection == RED_REMOTE_RED_SCORE       ||
                        selection == GREEN_REMOTE_UNDO          ||
                        selection == RED_REMOTE_UNDO ) {
                        std::cout << "selection: " << selection << " triggered the done flag, exiting while loop..." << std::endl;
                        done = true;
                    } else { 
                        // delay 250ms
                        std::cout << "sleeping 250ms..." << std::endl; 
                        GameTimer::gameDelay( 250 );
                    }
                }
            }
        }
        int serve_flag = 0;
        serve_flag = remoteLocker.playerNotServing( selection );
        print( "*** serve_flag: " << serve_flag << " ***");
        if ( serve_flag ) {
            print( "*** Warning: player not serving! ***" );
            continue;
        }
        print( "setting player button to selection: " << selection << " before calling loopGame()..." );
        if ( selection == GREEN_REMOTE_GREEN_SCORE || 
             selection == GREEN_REMOTE_RED_SCORE   ||
             selection == RED_REMOTE_GREEN_SCORE   ||
             selection == RED_REMOTE_RED_SCORE ) {
            // if remote pairing, write the words.  if not, snap out of the loop

            if ( selection == GREEN_REMOTE_GREEN_SCORE || selection == RED_REMOTE_GREEN_SCORE ) {
                print( "*** \n\n\nGreen player scored ***\n\n\n" );
                selection = 1; // Player 1 ( GREEN ) score // // flip the player score flag
            } else if ( selection == GREEN_REMOTE_RED_SCORE || selection == RED_REMOTE_RED_SCORE ) {
                print( "\n\n\n*** Red player scored ***\n\n\n" );
                selection = 2; // Player 2 ( RED ) score // // flip the player score flag
            }
            gameObject->playerScore( selection );  // flip the player score flag
        } else if ( selection == GREEN_REMOTE_UNDO || selection == RED_REMOTE_UNDO ) {
                print( "\n\n\n*** Undo ***\n\n\n" );
                gameObject->undo();
        } else {
            std::cout << "\n\n\n*** Invalid selection ***\n\n\n" << std::endl;
            sleep( SCORE_DELAY );
            continue;
        }
        sleep( SCORE_DELAY );
        gameObject->loopGame();  // handle the player score flag
        loop_count++;
        std::map<int, int> _player1_set_history = gameState->getPlayer1SetHistory();
        std::map<int, int> _player2_set_history = gameState->getPlayer2SetHistory();
        // print( "end /*/// Begin Game Loop ///*/ ***" );
    } ///////// End Game Loop /////////
}
```
# p.s.
Please at least retain the format of the original code, that part is good.  I like spaces before and after my parentheses for example.