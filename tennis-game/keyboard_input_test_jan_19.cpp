/*************************************************************
 * Keyboard Input Implementation with Timeout
 * Demonstration of how to seamlessly swap out with remote input
 *
 * Compile Example (assuming C++17 or later):
 *    g++ -g -std=c++17 main.cpp -o main
 * Run:
 *    ./main
 *************************************************************/

#include <iostream>
#include <thread>
#include <chrono>
#include <limits>
#include <stdexcept>
#include <csignal>
#include <atomic>

 /****************************************************
  *  Interfaces (Already Provided in Your System)
  *  (Listed here ONLY for completeness in this example;
  *   you mentioned you have them in your codebase, so
  *   normally, you wouldn't redefine them.)
  ****************************************************/
class IInput {
public:
    virtual ~IInput() = default;
    virtual int getInput() = 0;
};

class KeyboardInput : public IInput {
public:
    /**
     * @brief Gets keyboard input with timeout functionality
     *
     * This method reads user keyboard input in a non-blocking way with timeout handling.
     * It continuously polls for input until either valid input is received or the timeout period expires.
     *
     * Operation Flow:
     * 1. Starts a timer to track the timeout period
     * 2. Enters a polling loop that:
     *    - Checks if input is available using cin.rdbuf()->in_avail()
     *    - If input exists, reads and validates it
     *    - If no input, checks if timeout has occurred
     *    - Sleeps briefly between checks to prevent CPU overuse
     *
     * Input Validation:
     * - Checks for valid integer input
     * - Validates input against allowed values (1, 2, or 9)
     * - Handles and clears invalid input
     *
     * Timeout Handling:
     * - Uses chronological timing to enforce timeout
     * - Returns KEYBOARD_TIMEOUT_VALUE (-1) if timeout occurs
     * - Default timeout is 4000ms (4 seconds), configurable via setTimeout()
     *
     * @return int Valid input value (1, 2, or 9) or KEYBOARD_TIMEOUT_VALUE (-1) on timeout
     *
     * Example Usage:
     * @code
     * KeyboardInput input;
     * input.setTimeout(5000);  // Set 5 second timeout
     * int result = input.getInput();
     * if (result == KEYBOARD_TIMEOUT_VALUE) {
     *     // Handle timeout
     * } else {
     *     // Process valid input
     * }
     * @endcode
     */
    int getInput() override;

    /**
     * @brief Sets the timeout duration for keyboard input
     *
     * @param milliseconds The timeout duration in milliseconds (default is 10000)
     */
    void setTimeout( unsigned long milliseconds ) { _timeout_ms = milliseconds; }

private:
    bool validateInput( int selection );

private:
    unsigned long _timeout_ms = 4000; // Default 10 second timeout
};


#define KEYBOARD_TIMEOUT_VALUE -1

#include <fcntl.h>
#include <termios.h>
#include "KeyboardInput.h"

int KeyboardInput::getInput() {
    using namespace std::chrono;

    // Save terminal settings to restore later
    struct termios oldt, newt;
    tcgetattr( STDIN_FILENO, &oldt );
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO ); // Disable canonical mode and echo
    tcsetattr( STDIN_FILENO, TCSANOW, &newt );

    // Set stdin to non-blocking mode
    int old_flags = fcntl( STDIN_FILENO, F_GETFL, 0 );
    fcntl( STDIN_FILENO, F_SETFL, old_flags | O_NONBLOCK );

    auto start = steady_clock::now();
    int selection = 0;

    try {
        std::string inputBuffer;

        while ( true ) {
            // Check for timeout
            auto now = steady_clock::now();
            auto elapsedMs = duration_cast< milliseconds >( now - start ).count();
            if ( elapsedMs >= _timeout_ms ) {
                std::cout << "Keyboard input timed out after " << _timeout_ms / 1000 << " seconds..." << std::endl;
                return KEYBOARD_TIMEOUT_VALUE;
            }

            // Read input from stdin if available
            char ch;
            while ( read( STDIN_FILENO, &ch, 1 ) > 0 ) {
                if ( ch == '\n' ) { // End of input
                    try {
                        selection = std::stoi( inputBuffer );
                        if ( validateInput( selection ) ) {
                            std::cout << "Valid input received: " << selection << std::endl;
                            return selection;
                        }
                        else {
                            std::cout << "Invalid selection. Please enter 1, 2, or 9." << std::endl;
                            inputBuffer.clear();
                        }
                    }
                    catch ( const std::invalid_argument& ) {
                        std::cout << "Invalid input. Please enter a number." << std::endl;
                        inputBuffer.clear();
                    }
                }
                else {
                    inputBuffer += ch;
                }
            }

            // Small sleep to reduce CPU usage
            std::this_thread::sleep_for( milliseconds( 50 ) );
        }
    }
    catch ( ... ) {
        // Restore original terminal settings in case of an exception
        tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
        fcntl( STDIN_FILENO, F_SETFL, old_flags );
        throw;
    }

    // Restore original terminal settings
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
    fcntl( STDIN_FILENO, F_SETFL, old_flags );

    return KEYBOARD_TIMEOUT_VALUE;
}

bool KeyboardInput::validateInput( int selection )
{
    // Example of validating input
    // Here we say valid selections are 1,2,9, but adapt to your own game logic
    // If you want to allow any integer, just return true.
    return ( selection == 1 || selection == 2 || selection == 9 );
}

/****************************************************
 *  Minimal Stubs for Other Components
 *  (So we can compile and run a simple demo in main)
 ****************************************************/

 // We'll create a simple global signal status for demonstration
std::atomic<int> gSignalStatus{ 0 };

// A dummy signal handler that matches the signature we might use in a real system
void signalHandlerStub( int signal )
{
    gSignalStatus = signal;
}

// A stub game state enum to demonstrate usage
enum EnumGameState {
    PAIRING_MODE = 0,
    SLEEP_MODE = 1,
    AFTER_SLEEP_MODE = 2,
    REGULAR_BEFORE_SCORES,
    REGULAR_AFTER_SCORES,
    // ... add more as needed ...
};

// A dummy interface for demonstration (you said you have your own)
class IGameState {
public:
    virtual ~IGameState() = default;
    virtual bool gameRunning() const = 0;
    virtual void setCurrentAction( EnumGameState action ) = 0;
    virtual EnumGameState getCurrentAction() const = 0;
};

// A trivial implementation to let us compile
class StubGameState : public IGameState {
public:
    StubGameState() : _action( PAIRING_MODE ), _running( true ) {}

    bool gameRunning() const override { return _running; }
    void setCurrentAction( EnumGameState action ) override { _action = action; }
    EnumGameState getCurrentAction() const override { return _action; }

    // Optional: to end the loop
    void endGame() { _running = false; }

private:
    EnumGameState _action;
    bool _running;
};

// A stub game object interface for demonstration (you have your own)
class IGameObject {
public:
    virtual ~IGameObject() = default;
};

// A trivial implementation to let us compile
class StubGameObject : public IGameObject {
public:
    StubGameObject() {}
    ~StubGameObject() override = default;
};

/****************************************************
 *  Example of how you might wire everything together
 ****************************************************/
int main()
{
#define KEYBOARD_INPUT_TIMEOUT_DURATION 4000
    // Install a simple signal handler
    std::signal( SIGINT, signalHandlerStub );

    // Create stubs for demonstration
    StubGameObject gameObject;
    StubGameState gameState;

    // Create our KeyboardInput for user input
    KeyboardInput keyboardInput;
    // Set the default or custom timeout
    keyboardInput.setTimeout( KEYBOARD_INPUT_TIMEOUT_DURATION );

    // Show usage
    std::cout << "----- Tennis Game (Keyboard Input Demo) -----\n";
    std::cout << "This demo will time out after " << KEYBOARD_INPUT_TIMEOUT_DURATION / 1000 << " seconds if no input.\n";
    std::cout << "Valid entries: 1, 2, or 9 (as an example).\n";
    std::cout << "Press Ctrl+C to exit.\n\n";

    try {
        while ( gameState.gameRunning()) {
            std::cout << "\nPlease enter your selection (1, 2, or 9): ";
            int selection = keyboardInput.getInput();

            if ( selection != KEYBOARD_TIMEOUT_VALUE ) {
                std::cout << "[MAIN] You entered: " << selection << "\n";
            }
        }
    }
    catch ( const std::runtime_error& e ) {
        std::cerr << "[MAIN] Error: " << e.what() << std::endl;
    }

    std::cout << "[MAIN] Exiting.\n";
    return 0;
}


// Here is the output of this code:
// ```bash  
// adamsl@DESKTOP-SHDBATI:~/rpi-rgb-led-matrix/tennis-game$ ./keyboard_input_test_jan_19 
// ----- Tennis Game (Keyboard Input Demo) -----
// This demo will time out after 4 seconds if no input.
// Valid entries: 1, 2, or 9 (as an example).
// Press Ctrl+C to exit.


// Please enter your selection (1, 2, or 9): Keyboard input timed out after 4 seconds...

// Please enter your selection (1, 2, or 9): Keyboard input timed out after 4 seconds...

// Please enter your selection (1, 2, or 9): Keyboard input timed out after 4 seconds...

// Please enter your selection (1, 2, or 9): Keyboard input timed out after 4 seconds...

// Please enter your selection (1, 2, or 9): Keyboard input timed out after 4 seconds...

// Please enter your selection (1, 2, or 9): Keyboard input timed out after 4 seconds...

// Please enter your selection (1, 2, or 9): Keyboard input timed out after 4 seconds...
// ```

// This continues forever no matter what key I press.  And when I press keys, nothing shows up.  I need the input returned and the timer stopped when the input is made.

// Please rewrite the ```cpp int KeyboardInput::getInput() {``` to fix this.
