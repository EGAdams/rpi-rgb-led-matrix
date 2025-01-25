#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include "RemoteInputWithTimer.h"
#include "../TennisConstants/TennisConstants.h"

RemoteInputWithTimer::RemoteInputWithTimer( Blinker* blinker, Inputs* inputs, unsigned long timeout_ms )
    : IInputWithTimer( blinker, timeout_ms ), _inputs( inputs ) { // Call the correct base class constructor and initialize _inputs
    print( "RemoteInputWithTimer constructor called, checking blinker... " );
    if ( _blinker == nullptr ) {
        print( "*** ERROR: Blinker is null, exiting... ***" );
        exit( 1 );
    } else {
        print( "*** Blinker is not null, continuing... ***" );
    }
    std::cout << "RemoteInputWithTimer constructor called" << std::endl; }

RemoteInputWithTimer::~RemoteInputWithTimer() {}

int RemoteInputWithTimer::getInput() {
    using namespace std::chrono;
    auto startTime = steady_clock::now();
    unsigned long elapsedTimeMs = 0;
    unsigned long sleep_start = GameTimer::gameMillis(); // Mark start time with game timer
    int selection;
    bool done = false;
    print( "starting blinker from within RemoteInputWithTimer..." );
    if ( _blinker == nullptr ) {
        print( "*** ERROR: Blinker is null, exiting... ***" );
        exit( 1 );
    } else {
        print( "*** Blinker is not null, continuing... ***" );
    }
    _blinker->start();
    print( "getting input from within RemoteInputWithTimer..." );
    if ( REMOTE_INPUT == 1 ) {  // 122224
        /*// if the selection is never one of the valid remote inputs, then we will never exit the while loop! // 011925
         * there is no timer here, the agent says.  in the future we will take out this comment and ask a new employee a question about this dillemma.  we will ask them to explain why this is a bad design because it locks up the system. meaning,  so how do we fix this?
         * the timer that would allow us to break out of this while loop is not in the scope of this while loop! we need either to make a timer in here, or pass in a timer object to this function.  */
         // TODO: add an outside timer that will break out of this while loop.
        while ( !done ) {
            auto now = steady_clock::now();
            elapsedTimeMs = duration_cast< milliseconds >( now - startTime ).count();

            if ( elapsedTimeMs >= _timeout_ms ) {
                std::cout << "Keyboard input timed out after " << _timeout_ms / 1000 << " seconds." << std::endl;
                return _timeout_ms; // <--<< this is where we break out of the loop!  the other input with timer is missing this.
            }
            print( "*** reading selection from inputs... ***" ); // 122224
            selection = _inputs->read_mcp23017_value();  // this actually does have a while. // 011925
            std::cout << "read selection from inputs: " << selection << std::endl; // but it
            if ( selection == GREEN_REMOTE_GREEN_SCORE ||                         // isnt as bad as this
                 selection == GREEN_REMOTE_RED_SCORE ||                         // one because it always returns a value
                 selection == RED_REMOTE_GREEN_SCORE ||                         // 011925
                 selection == RED_REMOTE_RED_SCORE ) {
                std::cout << "selection: " << selection << " triggered the done flag, exiting while loop..." << std::endl;
                done = true;
            }
            else {
                // delay 250ms
                std::cout << "sleeping 250ms..." << std::endl; // so we end up reading this over and over // 011925
                GameTimer::gameDelay( 250 );
            }
        }
    }
    else if ( REMOTE_INPUT == 0 ) {  // menu mode // 122224
        std::cout << "Enter your selection: ";
        std::cin >> selection;
        print( "made seleciton in RemoteInputWithTimer::getInput()... " );
        print( "selection: " << selection );

    }
    else {
        std::cout << "*** ERROR: Invalid input mode in RemoteInputWithTimer Object getInput() method. ***\n";
        exit( 1 );
    }
    _blinker->stop();
    unsigned long sleep_end = GameTimer::gameMillis(); // Mark end time with game timer
    _time_slept = sleep_end - sleep_start;
    return selection;
}
