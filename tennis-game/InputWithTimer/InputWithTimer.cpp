#include "InputWithTimer.h"
#include <iostream>
#include "../TennisConstants/TennisConstants.h"

InputWithTimer::InputWithTimer( Blinker* blinker, Inputs* inputs ) : _blinker( blinker ), _inputs( inputs ) {
    _time_slept = 0;
}
InputWithTimer::~InputWithTimer() {}

# define REMOTE_INPUT 1
# define MANUAL_INPUT 0

int InputWithTimer::getInput() {
    unsigned long sleep_start = GameTimer::gameMillis(); // Mark start time with game timer
    int selection;
    bool done = false;  
    print( "starting blinker from within InputWithTimer..." );
    _blinker->start();
    print( "getting input from within InputWithTimer..." );
    if ( REMOTE_INPUT == 1 ) {  
        while ( !done ) {                           // remote mode
            selection = _inputs->read_mcp23017_value();
            std::cout << "read selection from inputs: " << selection << std::endl;
            if ( selection == 7 || selection == 11 ) {
                std::cout << "selection: " << selection << " triggered the done flag, exiting while loop..." << std::endl;
                done = true;
            } else { 
                // delay 250ms
                std::cout << "sleeping 250ms..." << std::endl; 
                GameTimer::gameDelay( 250 );
            }
        }
    } else if ( MANUAL_INPUT == 1 ) {               // menu mode
        std::cout << "Enter your selection: ";
        std::cin >> selection;
    } else {
        std::cout << "*** ERROR: Invalid input mode in InputWithTimer Object getInput() method. ***\n";
        exit( 1 );
    }
    _blinker->stop();
    unsigned long sleep_end = GameTimer::gameMillis(); // Mark end time with game timer
    _time_slept = sleep_end - sleep_start;
    return selection;
}

unsigned long InputWithTimer::getTimeSlept() {
    return _time_slept;
}

