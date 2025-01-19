#include <iostream>
#include <thread>
#include <chrono>
#include <limits>
#include <stdexcept>
#include <csignal>
#include <atomic>
#include "../IInput/IInput.h"
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

    KeyboardInput( unsigned long milliseconds = 4000 ) : _timeout_ms( milliseconds ) {}
    ~KeyboardInput();

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
