#include "GameTimer.h"
#include "../TennisConstants/TennisConstants.h"

GameTimer::GameTimer() {}
GameTimer::~GameTimer(){}

void GameTimer::gameDelay( int milliseconds ) {
    // return; // TODO: REMOVE THIS HARD CODE!
    // print the name of the function before this on the call stack
    // std::string functionName = __FUNCTION__;
    // std::cout << "Function: " << functionName << std::endl;


    // print( " *** Game Delay: << " << milliseconds << " *** " );
    std::this_thread::sleep_for( std::chrono::milliseconds( milliseconds )); 
}

void GameTimer::sleep_until( int milliseconds ) {}

unsigned long GameTimer::gameMillis() {
    std::chrono::milliseconds ms =
    std::chrono::duration_cast<std::chrono::milliseconds>(
    std::chrono::system_clock::now().time_since_epoch());
    return ms.count(); }