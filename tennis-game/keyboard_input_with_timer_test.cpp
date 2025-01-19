#include <iostream>
#include <memory>
#include <chrono>
#include <thread>
#include <atomic>
#include <limits>
#include <csignal>
#include <stdexcept>
#include <fcntl.h>
#include <termios.h>

/****************************************************
 * Interfaces for KeyboardInput
 ****************************************************/
class IInput {
public:
    virtual ~IInput() = default;
    virtual int getInput() = 0;
};

/****************************************************
 * KeyboardInput Class Implementation
 * Time the waiting for keyboard input for the purpose
 * of a timeout simulation in the case of using
 * the remote inputs.
 * 
 * in the case of the remote inputs, we time the waiting
 * for remote input.
 ****************************************************/
class KeyboardInput : public IInput {
public:
    int getInput() override;
    void setTimeout( unsigned long milliseconds ) { _timeout_ms = milliseconds; }

private:
    bool validateInput( int selection ) {
        return ( selection == 1 || selection == 2 || selection == 9 );
    }

    unsigned long _timeout_ms = 4000; // Default timeout (4 seconds)
};

#define KEYBOARD_TIMEOUT_VALUE -1

int KeyboardInput::getInput() {
    using namespace std::chrono;

    struct termios oldt, newt;
    tcgetattr( STDIN_FILENO, &oldt );
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newt );

    int old_flags = fcntl( STDIN_FILENO, F_GETFL, 0 );
    fcntl( STDIN_FILENO, F_SETFL, old_flags | O_NONBLOCK );

    auto start = steady_clock::now();
    int selection = 0;
    std::string inputBuffer;

    try {
        while ( true ) {
            auto now = steady_clock::now();
            auto elapsedMs = duration_cast< milliseconds >( now - start ).count();
            if ( elapsedMs >= _timeout_ms ) {
                std::cout << "Keyboard input timed out after " << _timeout_ms / 1000 << " seconds...\n";
                return KEYBOARD_TIMEOUT_VALUE;
            }

            char ch;
            while ( read( STDIN_FILENO, &ch, 1 ) > 0 ) {
                if ( ch == '\n' ) {
                    try {
                        selection = std::stoi( inputBuffer );
                        if ( validateInput( selection ) ) {
                            std::cout << "Valid input received: " << selection << std::endl;
                            return selection;
                        }
                        else {
                            std::cout << "Invalid selection. Please enter 1, 2, or 9.\n";
                            inputBuffer.clear();
                        }
                    }
                    catch ( const std::invalid_argument& ) {
                        std::cout << "Invalid input. Please enter a number.\n";
                        inputBuffer.clear();
                    }
                }
                else {
                    inputBuffer += ch;
                }
            }

            std::this_thread::sleep_for( milliseconds( 50 ) );
        }
    }
    catch ( ... ) {
        tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
        fcntl( STDIN_FILENO, F_SETFL, old_flags );
        throw;
    }

    tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
    fcntl( STDIN_FILENO, F_SETFL, old_flags );

    return KEYBOARD_TIMEOUT_VALUE;
}

/****************************************************
 * Interfaces and Mock Classes for InputWithTimer
 ****************************************************/
class IInputs {
public:
    virtual ~IInputs() = default;
    virtual int read_mcp23017_value() { return 9; } // Mock: Always return 9
};

class IPairingBlinker {
public:
    virtual ~IPairingBlinker() = default;
};

class IInputWithTimer {
public:
    virtual ~IInputWithTimer() = default;
    virtual int getInput() = 0;
    virtual long getTimeSlept() const = 0;
};

/****************************************************
 * InputWithTimer Class Implementation
 ****************************************************/
class InputWithTimer : public IInputWithTimer {
    IPairingBlinker* m_pairingBlinker;
    IInputs* m_inputs;
    long m_timeSleptMs{ 0 };

public:
    InputWithTimer( IPairingBlinker* pairingBlinker, IInputs* inputs )
        : m_pairingBlinker( pairingBlinker ), m_inputs( inputs ) {
    }

    int getInput() override {
#define REMOTE_INPUT 0
        using namespace std::chrono;
        auto start = steady_clock::now();
        int selection = -1;

        if ( REMOTE_INPUT == 1 ) {
            selection = m_inputs->read_mcp23017_value();
        } else {
            auto endWaitTime = steady_clock::now() + seconds( 4 );
            while ( true ) {
                if ( std::cin.rdbuf()->in_avail() > 0 ) {
                    std::cin >> selection;
                    break;
                }
                if ( steady_clock::now() > endWaitTime ) {
                    selection = -1;
                    break;
                }
                std::this_thread::sleep_for( milliseconds( 50 ) );
            }
        }

        m_timeSleptMs = duration_cast< milliseconds >( steady_clock::now() - start ).count();
        return selection;
    }

    long getTimeSlept() const override {
        return m_timeSleptMs;
    }
};

/****************************************************
 * Main Function for Testing
 ****************************************************/
#define REMOTE_INPUT 0

int main() {
    KeyboardInput keyboardInput;
    keyboardInput.setTimeout( 4000 );

    IPairingBlinker* pairingBlinker = new IPairingBlinker();
    IInputs* inputs = new IInputs();
    InputWithTimer inputWithTimer( pairingBlinker, inputs );

    std::cout << "----- Keyboard Input Test -----\n";
    std::cout << "Please enter your selection (1, 2, or 9): ";
    int kbResult = keyboardInput.getInput();
    if ( kbResult == KEYBOARD_TIMEOUT_VALUE ) {
        std::cout << "Keyboard input timed out.\n";
    }
    else {
        std::cout << "You entered: " << kbResult << "\n";
    }

    std::cout << "\n----- InputWithTimer Test -----\n";
    std::cout << "Please enter your selection (1, 2, or 9): ";
    int timerResult = inputWithTimer.getInput();
    if ( timerResult == -1 ) {
        std::cout << "InputWithTimer timed out.\n";
    }
    else {
        std::cout << "You entered: " << timerResult << ", Time spent: " << inputWithTimer.getTimeSlept() << " ms\n";
    }

    delete pairingBlinker;
    delete inputs;

    return 0;
}
