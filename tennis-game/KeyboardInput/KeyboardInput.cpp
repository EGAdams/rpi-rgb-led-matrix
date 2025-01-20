#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>

class KeyboardInput {
private:
    int _timeout_ms; // Timeout in milliseconds
    std::chrono::steady_clock::time_point _startTime; // Start time for input polling
    int _elapsedTimeMs; // Elapsed time in milliseconds

    /**
     * @brief Validates the user input against allowed values.
     * @param input The input value to validate.
     * @return True if the input is valid (1, 2, or 9), false otherwise.
     */
    bool validateInput( int input ) const {
        return input == 1 || input == 2 || input == 9;
    }

    /**
     * @brief Configures the terminal for non-blocking input.
     * @param oldt The structure to store the old terminal settings.
     * @return The original file descriptor flags to restore later.
     */
    int configureTerminal( struct termios& oldt ) const {
        struct termios newt = oldt;
        newt.c_lflag &= ~( ICANON | ECHO ); // Disable canonical mode and echo
        tcsetattr( 0, TCSANOW, &newt );

        int old_flags = fcntl( 0, F_GETFL, 0 );
        fcntl( 0, F_SETFL, old_flags | O_NONBLOCK );
        return old_flags;
    }

    /**
     * @brief Restores the original terminal settings.
     * @param oldt The original terminal settings.
     * @param old_flags The original file descriptor flags.
     */
    void restoreTerminal( const struct termios& oldt, int old_flags ) const {
        tcsetattr( 0, TCSANOW, &oldt );
        fcntl( 0, F_SETFL, old_flags );
    }

public:
    static const int KEYBOARD_TIMEOUT_VALUE = -1;

    /**
     * @brief Constructor for KeyboardInput.
     * @param timeout_ms The timeout duration in milliseconds.
     */
    explicit KeyboardInput( int timeout_ms = 4000 )
        : _timeout_ms( timeout_ms ), _elapsedTimeMs( 0 ) {
    }

    /**
     * @brief Gets keyboard input with timeout functionality.
     * @return The valid input value (1, 2, or 9) or a timeout value (-1).
     */
    int getInput() {
        using namespace std::chrono;

        struct termios oldt;
        tcgetattr( 0, &oldt );
        int old_flags = configureTerminal( oldt );

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
                    return KEYBOARD_TIMEOUT_VALUE;
                }

                char ch;
                while ( read( 0, &ch, 1 ) > 0 ) {
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
            restoreTerminal( oldt, old_flags );
            throw;
        }

        restoreTerminal( oldt, old_flags );
        return KEYBOARD_TIMEOUT_VALUE;
    }

    /**
     * @brief Gets the elapsed time since polling started.
     * @return The elapsed time in milliseconds.
     */
    int getTimeSlept() const {
        return _elapsedTimeMs;
    }
};
