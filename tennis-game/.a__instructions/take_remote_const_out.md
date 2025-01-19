
```cpp
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
```