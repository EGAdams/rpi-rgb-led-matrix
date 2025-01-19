#include "KeyboardInput.h"
#define KEYBOARD_TIMEOUT_VALUE -1
#include <fcntl.h>
#include <termios.h>

KeyboardInput( Blinker* blinker, Inputs* inputs ) : _blinker( blinker ), _inputs( inputs ) {}

int KeyboardInput::getInput() {
    using namespace std::chrono;

    // Save terminal settings to restore later
    struct termios oldt, newt;
    tcgetattr(0, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echo
    tcsetattr(0, TCSANOW, &newt);

    // Set stdin to non-blocking mode
    int old_flags = fcntl(0, F_GETFL, 0);
    fcntl(0, F_SETFL, old_flags | O_NONBLOCK);

    auto start = steady_clock::now();
    int selection = 0;

    try {
        std::string inputBuffer;

        while (true) {
            // Check for timeout
            auto now = steady_clock::now();
            auto elapsedMs = duration_cast<milliseconds>(now - start).count();
            if (elapsedMs >= _timeout_ms) {
                std::cout << "Keyboard input timed out after " << _timeout_ms / 1000 << " seconds..." << std::endl;
                return KEYBOARD_TIMEOUT_VALUE;
            }

            // Read input from stdin if available
            char ch;
            while (read(0, &ch, 1) > 0) {
                if (ch == '\n') { // End of input
                    try {
                        selection = std::stoi(inputBuffer);
                        if (validateInput(selection)) {
                            std::cout << "Valid input received: " << selection << std::endl;
                            return selection;
                        }
                        else {
                            std::cout << "Invalid selection. Please enter 1, 2, or 9." << std::endl;
                            inputBuffer.clear();
                        }
                    }
                    catch (const std::invalid_argument&) {
                        std::cout << "Invalid input. Please enter a number." << std::endl;
                        inputBuffer.clear();
                    }
                }
                else {
                    inputBuffer += ch;
                }
            }

            // Small sleep to reduce CPU usage
            std::this_thread::sleep_for(milliseconds(50));
        }
    }
    catch (...) {
        // Restore original terminal settings in case of an exception
        tcsetattr(0, TCSANOW, &oldt);
        fcntl(0, F_SETFL, old_flags);
        throw;
    }

    // Restore original terminal settings
    tcsetattr(0, TCSANOW, &oldt);
    fcntl(0, F_SETFL, old_flags);

    return KEYBOARD_TIMEOUT_VALUE;
}

bool KeyboardInput::validateInput( int selection )
{
    // Example of validating input
    // Here we say valid selections are 1,2,9, but adapt to your own game logic
    // If you want to allow any integer, just return true.
    return ( selection == 1 || selection == 2 || selection == 9 );
}
