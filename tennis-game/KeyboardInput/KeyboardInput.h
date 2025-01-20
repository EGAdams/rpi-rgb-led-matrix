#ifndef KEYBOARD_INPUT_H
#define KEYBOARD_INPUT_H

#include <chrono>
#include "IInputWithTimer.h"

/**
 * @brief A class for handling non-blocking keyboard input with timeout functionality.
 * 
 * KeyboardInput allows for polling keyboard input while enforcing a timeout period. 
 * It includes methods to get elapsed time during the polling process.
 */
class KeyboardInput : public IInputWithTimer {
public:
    /**
     * @brief Constructor for KeyboardInput.
     * @param milliseconds The timeout duration in milliseconds (default is 4000).
     */
    explicit KeyboardInput(unsigned long milliseconds = 4000);

    /**
     * @brief Destructor for KeyboardInput.
     */
    ~KeyboardInput();

    /**
     * @brief Gets keyboard input with timeout functionality.
     * 
     * Continuously polls for keyboard input until valid input is received or timeout occurs.
     * Valid input values are 1, 2, or 9. Returns a timeout value if no input is received.
     * 
     * @return int The valid input value or a timeout value (-1).
     */
    int getInput() override;

    /**
     * @brief Sets the timeout duration for keyboard input.
     * @param milliseconds The timeout duration in milliseconds (default is 4000).
     */
    void setTimeout(unsigned long milliseconds);

    /**
     * @brief Retrieves the elapsed time since input polling started.
     * @return int The elapsed time in milliseconds.
     */
    int getTimeSlept() const;

private:
    /**
     * @brief Validates the user input against allowed values.
     * @param selection The input value to validate.
     * @return bool True if the input is valid, false otherwise.
     */
    bool validateInput(int selection);

private:
    unsigned long _timeout_ms; ///< Timeout duration in milliseconds.
    std::chrono::steady_clock::time_point _startTime; ///< Start time for input polling.
    int _elapsedTimeMs; ///< Elapsed time in milliseconds.
};

#endif // KEYBOARD_INPUT_H
