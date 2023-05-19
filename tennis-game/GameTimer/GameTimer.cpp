#include "GameTimer.h"
#include "Arduino.h"

#if defined _WIN32 || defined _WIN64
#include <chrono>
#include <thread>
#endif

GameTimer::GameTimer() {
    // cout << "GameTimer::GameTimer() called..." << endl;
}

void GameTimer::gameDelay( int milliseconds ) {
#if defined _WIN32 || defined _WIN64
    // std::cout << "Sleeping for " << milliseconds << " milliseconds" << std::endl;
    std::this_thread::sleep_for( std::chrono::milliseconds( milliseconds ));
#else
    delay( milliseconds );
#endif
}
GameTimer::~GameTimer() {
    // cout << "GameTimer::GameTimer() called..." << endl;
}

void GameTimer::sleep_until( int milliseconds ) {
    // cout << "Sleeping until " << milliseconds << " milliseconds" << endl;
    // std::this_thread::sleep_until(std::chrono::system_clock::now() +
    //                               std::chrono::milliseconds(milliseconds));
    // sleep_until( milliseconds );
}

unsigned long GameTimer::gameMillis() {
    #if defined _WIN32 || defined _WIN64
        std::chrono::milliseconds ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch());
        return ms.count();
    #else
        return millis();
    #endif
}
