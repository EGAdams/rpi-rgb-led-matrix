#include "ScoreboardBlinker.h"
#include <chrono>
#include <thread>

ScoreboardBlinker::ScoreboardBlinker( ScoreBoard* scoreBoard ) :
    _scoreboard( scoreBoard ) {
        _time_slept = 0;
    }

ScoreboardBlinker::~ScoreboardBlinker() { stop(); }

void ScoreboardBlinker::blinkLoop() {
    while ( !should_stop ) {
        blinkTennisBall( true );
        std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
        if ( _time_slept >  MAX_SLEEP ) {
            print( "UNDO: Reset Game        Player1 Score: Reset Game  Player2 Score: Reset Game slept " << _time_slept++ << " seconds." );    
        }
        blinkTennisBall( false );
        std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
        if (_time_slept <= MAX_SLEEP) {
            print( "UNDO: Enter last match  Player1 Score: Reset Game  Player2 Score: Reset Game   slept " << _time_slept++ << " seconds.");    
        }
    }
}

void ScoreboardBlinker::blinkTennisBall( bool show ) {
    // Implement the logic to show/hide the tennis ball on the scoreboard
    // This will depend on your existing scoreboard implementation
    if ( show ) {
        // std::cout << "drawing yellow period... " << std::endl;
        _scoreboard->drawYellowPeriod();
    }
    else {
        _scoreboard->clearScreen();
    }
}

void ScoreboardBlinker::start() {
    should_stop = false;
    blink_thread = std::thread( &ScoreboardBlinker::blinkLoop, this );
}

void ScoreboardBlinker::stop() {
    should_stop = true;
    if ( blink_thread.joinable() ) {
        blink_thread.join();
    }
}

