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

/**
 * @brief Constructor for KeyboardInputInputWithTimer.
 * @param timeout_ms The timeout duration in milliseconds.
 */
KeyboardInputWithTimer::KeyboardInputWithTimer(Blinker* blinker, unsigned long timeout_ms)
    : IInputWithTimer(blinker, timeout_ms), _elapsedTimeMs(0)  // Call the correct base class constructor
{}  // initialize elapsedTimeMs to 0
/**
 * @brief Destructor for KeyboardInputInputWithTimer.
 * Restores the terminal settings.
 */
KeyboardInputWithTimer::~KeyboardInputWithTimer() {
    std::cout << "keyboard destructing without restoring the keyboard." << std::endl;
    // restoreTerminal( oldt, old_flags );
}

/**
 * @brief Validates the user input against allowed values.
 * @param input The input value to validate.
 * @return True if the input is valid (1, 2, or 9), false otherwise.
 */
bool KeyboardInputWithTimer::validateInput( int selection ) const {
    return selection == 1 || selection == 2 || selection == 9;
}

/**
 * @brief Configures the terminal for non-blocking input.
 * @param oldt The structure to store the old terminal settings.
 * @return The original file descriptor flags to restore later.
 */
int KeyboardInputWithTimer::_configureTerminal(struct termios& oldt) {
    struct termios newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echo
    tcsetattr(0, TCSANOW, &newt);

    int old_flags = fcntl(0, F_GETFL, 0);
    fcntl(0, F_SETFL, old_flags | O_NONBLOCK);
    return old_flags;
}

/**
 * @brief Restores the original terminal settings.
 * @param oldt The original terminal settings.
 * @param old_flags The original file descriptor flags.
 */
void KeyboardInputWithTimer::_restoreTerminal( const struct termios& oldt, int old_flags ) {
    tcsetattr( 0, TCSANOW, &oldt );
    fcntl( 0, F_SETFL, old_flags );
}


/**
 * @brief Gets keyboard input with timeout functionality.
 * @return The valid input value (1, 2, or 9) or a timeout value (-1).
 */
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
                std::cout << "Keyboard input timed out after " << _timeout_ms / 1000
                    << " seconds." << std::endl;
                return _elapsedTimeMs;    // <--<< this is where we break out of the loop!  the other input with timer is missing this.
            }

            char ch;
            while ( read( 0, &ch, 1 ) > 0 ) {        // this while isn't as bad as the outside one.  that's why we're checking the elapsed time.
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
