# What I want you to think about 
Please analyze the following code and think about how we can fix the problem of the system not listening to the keyboard and remote at the same time.
This `run_remote_keyboard` method is called when the game starts.  
# Start up source code
```cpp
#include <queue>
#include <chrono>
#include <functional>

// The new run_remote_keyboard method
void run_remote_keyboard(GameObject* gameObject, GameState* gameState, Reset* reset, Inputs* inputs) {
    print( "registering signal handler..." );
    std::signal(SIGINT, signalHandler);

    print( "calling gameObject->loopGame()..." );
    gameObject->loopGame();
    print( "sleeping for 1 second..." );
    std::this_thread::sleep_for(std::chrono::seconds(1));
    print( "done sleeping after calling gameObject->loopGame()..." );
    gameObject->getScoreBoard()->setLittleDrawerFont("fonts/8x13B.bdf");

    print( "constructing remotePairingScreen..." );
    RemotePairingScreen remotePairingScreen(gameObject->getScoreBoard());

    print( "constructing page blinker..." );
    PairingBlinker pairingBlinker(gameObject->getScoreBoard());
    bool is_on_pi = gameObject->getScoreBoard()->onRaspberryPi();

    print( "Initialize thread-safe input queue..." );
    ThreadSafeQueue<int> inputQueue;

    print( "Initialize input listeners..." );
    KeyboardInputListener keyboardListener(&inputQueue);
    RemoteInputListener remoteListener(&inputQueue, &pairingBlinker, inputs);

    print( "starting keyboard and remote listeners..." );
    keyboardListener.startListening(); // Start listening
    remoteListener.startListening();

    print( "entering main loop..." );
    while (gameState->gameRunning() && !stopListening.load()) {  // Main loop
        // Process inputs from the queue
        int selection = 0;
        while (inputQueue.dequeue(selection)) {
            print( "Handle the selection." );
            if (remotePairingScreen.inPairingMode() && is_on_pi && pairingBlinker.awake()) {
                if (selection == 7) {
                    remotePairingScreen.greenPlayerPressed();
                    pairingBlinker.setGreenPlayerPaired(true);
                } else if (selection == 11) {
                    remotePairingScreen.redPlayerPressed();
                    pairingBlinker.setRedPlayerPaired(true);
                } else {
                    print( "Invalid selection." );
                }
            } else if (gameState->getCurrentAction() == SLEEP_MODE) {
                // Handle sleep mode selections
                ScoreboardBlinker blinker(gameObject->getScoreBoard());
                InputWithTimer sleepInput(&blinker, inputs);
                if (selection == 7 || selection == 11 || sleepInput.getTimeSlept() > MAX_SLEEP * 1000) {
                    gameObject->resetMatch();
                    if (sleepInput.getTimeSlept() > MAX_SLEEP * 1000) {
                        gameObject->getHistory()->clearHistory();
                    }
                    continue;
                }
                gameObject->getScoreBoard()->clearScreen();
                gameObject->getScoreBoard()->update();
            } else {
                print( "Handle manual (keyboard) inputs" ); 
                switch (selection) {
                    case 1:
                    case 2:
                        gameObject->playerScore(selection);
                        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_AFTER_REMOTE_SCORE));
                        break;
                    case 20:
                        // Handle menu selection 20
                        std::cout << "Enter the message to write: ";
                        {
                            std::string message;
                            std::cin >> message;
                            gameObject->getScoreBoard()->clearScreen();
                            gameObject->getScoreBoard()->drawNewText(message, 5, 20);
                            GameTimer::gameDelay(1000);
                        }
                        break;
                    // Handle other menu selections as needed
                    default:
                        std::cout << "*** Invalid selection ***\n" << std::endl;
                        break;
                }
            }
        }

        print( "calling gameObject->loopGame()..." );
        gameObject->loopGame();
        std::this_thread::sleep_for(std::chrono::milliseconds(REMOTE_SPIN_DELAY));
    }
   
    stopListening.store(true);              // Signal listeners to stop
    keyboardListener.stopListeningInput();
    remoteListener.stopListeningInput();

    inputQueue.clear();                     // Clear the input queue
    print( "end of run remote_keyboard method. " );
}
```
The problem is that when this line of code is called:
```cpp
gameObject->loopGame();
```
The whole process hangs.
Note that ```cpp gameObject->loopGame();``` calls the `mode1()` method below.

## Helper functions
```c++
// Thread-safe queue implementation
template<typename T>
class ThreadSafeQueue {
private:
    std::queue<T> queue_;
    std::mutex mutex_;
    std::condition_variable cond_;
public:
    void enqueue(const T& value) {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            queue_.push(value);
        }
        cond_.notify_one();
    }

    bool dequeue(T& result) {
        std::unique_lock<std::mutex> lock(mutex_);
        while (queue_.empty() && !stopListening.load()) {
            cond_.wait(lock);
        }
        if (queue_.empty()) {
            return false;
        }
        result = queue_.front();
        queue_.pop();
        return true;
    }

    void clear() {
        std::lock_guard<std::mutex> lock(mutex_);
        while (!queue_.empty()) queue_.pop();
    }
};

// Input Listener Interface
class IInputListener {
public:
    virtual void startListening() = 0;
    virtual void stopListeningInput() = 0;
    virtual ~IInputListener() {}
};

// Keyboard Listener Implementation
class KeyboardInputListener : public IInputListener {
private:
    ThreadSafeQueue<int>* inputQueue_;
    std::thread listenerThread_;
    std::atomic<bool> listening_;
public:
    KeyboardInputListener(ThreadSafeQueue<int>* queue)
        : inputQueue_(queue), listening_(false) {}

    void startListening() override {
        listening_ = true;
        listenerThread_ = std::thread([this]() {
            while (listening_.load() && !stopListening.load()) {
                int input;
                if (std::cin >> input) {
                    inputQueue_->enqueue(input);
                } else {
                    // Handle EOF or invalid input
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
            }
        });
    }

    void stopListeningInput() override {
        listening_ = false;
        if (listenerThread_.joinable()) {
            listenerThread_.join();
        }
    }

    ~KeyboardInputListener() {
        stopListeningInput();
    }
};

// Remote Listener Implementation
class RemoteInputListener : public IInputListener {
private:
    ThreadSafeQueue<int>* inputQueue_;
    std::thread listenerThread_;
    std::atomic<bool> listening_;
    Inputs* inputs_;
    Blinker* blinker_;
public:
    RemoteInputListener(ThreadSafeQueue<int>* queue, Blinker* blinker, Inputs* inputs)
        : inputQueue_(queue), blinker_(blinker), inputs_(inputs), listening_(false) {}

    void startListening() override {
        listening_ = true;
        listenerThread_ = std::thread([this]() {
            InputWithTimer inputWithTimer(blinker_, inputs_);
            while (listening_.load() && !stopListening.load()) {
                int selection = inputWithTimer.getInput();
                inputQueue_->enqueue(selection);
                std::this_thread::sleep_for(std::chrono::milliseconds(REMOTE_SPIN_DELAY));
            }
        });
    }

    void stopListeningInput() override {
        listening_ = false;
        if (listenerThread_.joinable()) {
            listenerThread_.join();
        }
    }

    ~RemoteInputListener() {
        stopListeningInput();
    }
};

// Signal Handler
void signalHandler(int signum) {
    stopListening.store(true);
}


```

## This is the main function that is frequently called during this tennis game.
```cpp gameObject->loopGame(); ``` calls this method, so get ready for an infinite loop somewhere...
```cpp
void GameModes::mode1() { 
    print( "just inside mode1()... " );
    _gameState->setNow( GameTimer::gameMillis());
    print( "reading undo button..." );
    _inputs.readUndoButton();
    if ( _gameState->getUndo() == 1 ) {  // undo button pressed
        _gameState->setUndo( 0 );
        _undo.mode1Undo( _history );
    }
    print( "reading _inputs.read_mcp23017_value()..." );  // digital read on GPIO pins.  
    int button_read = _inputs.read_mcp23017_value();      // sets playerButton if tripped.
    print( "read button:" << button_read );
    _serveLeds.serveSwitch(); // if serveSwitch >= 2, serveSwitch = 0; and toggle serve variable
    _logger->setName( "mode1" );
    _mode1Functions.mode1ButtonFunction(); // <--------- ENTRY POINT --------------<<
    if ( _gameState->getCurrentAction() == SLEEP_MODE ) {
        print( "not running point flash because sleep mode has been detected..." );
    } else {
        _mode1Functions.pointFlash();
    }
}
```
when we get to this line, the program starts listening to the GPIO pins:
```cpp
int button_read = _inputs.read_mcp23017_value();      // sets playerButton if tripped.
```



## read_mcp23017_value()  // reads the MCP23017 GPIO pins
```c++
int Inputs::read_mcp23017_value() {
    int originalRemoteCode = _pinInterface->read_mcp23017_value();
    GameTimer::gameDelay( STEVE_DELAY ); // that delay steve was talking about...
    int freshRemoteCode = _pinInterface->read_mcp23017_value();
    if (( freshRemoteCode == originalRemoteCode ) && ( freshRemoteCode != UNKNOWN_REMOTE_BUTTON )) {  // known code and a match?
        print( "entering while reading remote codes..." );
        while(( freshRemoteCode == originalRemoteCode ) && ( freshRemoteCode != UNKNOWN_REMOTE_BUTTON )) {
            // std::cout <<  "inside while.  freshRemoteCode [" << std::to_string( freshRemoteCode ) << "]" << std::endl;
            GameTimer::gameDelay( REMOTE_READ_DELAY ); //  wait 250ms, then get a fresh one...
            // std::cout << "after delay within while reading getting fresh remote code again to verify" << std::endl;
            freshRemoteCode = _pinInterface->read_mcp23017_value();
            // std::cout << "after delay within while; freshRemoteCode [" << std::to_string( freshRemoteCode ) << "]" << std::endl;
        }
        std::cout << "exited while.  *** CODE IS VALID ***.  returning originalRemoteCode [" << std::to_string( originalRemoteCode ) << "]" << std::endl;
        return originalRemoteCode;
    } else {                        // false alarm.  we DO NOT have a matching measurement gameDelay( x ) apart.
        return UNKNOWN_REMOTE_BUTTON;
    }
 }
 ```

# Problem
When this while loop has been entered, since i don't have the remote with me, It is stuck in this loop.  The loop starts with this line of  code:
```cpp
 while(( freshRemoteCode == originalRemoteCode ) && ( freshRemoteCode != UNKNOWN_REMOTE_BUTTON )) {
```

The fresh remote codes are pulled from the _pinInterface->read_mcp23017_value() method, but we are not reading the keyboard at all.  With all of the C++ code and Thread management that has already been written above, we should be able to read the keyboard and the remote at the same time.

# Your Goal
Write the C++ code to fix this delema.
