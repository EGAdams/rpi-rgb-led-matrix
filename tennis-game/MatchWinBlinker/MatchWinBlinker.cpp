#include "MatchWinBlinker.h"
#define MATCH_WIN_OFFSET 12

MatchWinBlinker::MatchWinBlinker( ScoreBoard* scoreboard )
    : _scoreboard( scoreboard ), _shouldStop( false ), _running( false ) {
    _scoreboard->setLittleDrawerFont( "the_sets_numbers.bdf" );
}

MatchWinBlinker::~MatchWinBlinker() { stop(); }

void MatchWinBlinker::start() {
    if ( _running.load()) return;  // Prevent starting a new thread if already running

    // If a previous thread finished but wasn't joined, join it.
    if (_blinkThread.joinable()) {
        _blinkThread.join();
    }

    _shouldStop.store( false );
    _running.store( true );
    _blinkThread = std::thread( &MatchWinBlinker::blinkLoop, this );
}

void MatchWinBlinker::stop() {
    _shouldStop.store( true );
    if ( _blinkThread.joinable()) { _blinkThread.join(); }
    _running.store( false );
}

bool MatchWinBlinker::isRunning() const { return _running.load(); }

void MatchWinBlinker::blinkLoop() {
    std::cout << "MatchWinBlinker started." << std::endl;
    for ( int blink_count = 0; blink_count < MATCH_WIN_BLINK_COUNT; blink_count++ ) {
        if ( _shouldStop.load()) break;
        _scoreboard->clearScreen();
        _scoreboard->drawNewText( "Match", 13, 60 - MATCH_WIN_OFFSET );
        _scoreboard->drawNewText( "Win",   20, 75 - MATCH_WIN_OFFSET );
        _scoreboard->drawSets();
        GameTimer::gameDelay( MATCH_WIN_FLASH_DELAY );
        if ( _shouldStop.load()) break;
        _scoreboard->clearScreen();
        GameTimer::gameDelay( MATCH_WIN_FLASH_DELAY );
    }
    std::cout << "MatchWinBlinker completed." << std::endl;
    _running.store(false);
}
