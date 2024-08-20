#include "ScoreboardBlinker.h"
#include <chrono>
#include <thread>

ScoreboardBlinker::ScoreboardBlinker( ScoreBoard* scoreBoard ) : 
    _scoreboard( scoreBoard ) {}

ScoreboardBlinker::~ScoreboardBlinker() { stop();}

void ScoreboardBlinker::blinkLoop() {
    while (!should_stop) {
        blinkTennisBall(true);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        blinkTennisBall(false);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void ScoreboardBlinker::blinkTennisBall(bool show) {
    // Implement the logic to show/hide the tennis ball on the scoreboard
    // This will depend on your existing scoreboard implementation
    if ( show ) {
        _scoreboard->drawGreenPeriod();
    } else {
        _scoreboard->clearScreen();
    }
}

void ScoreboardBlinker::start() {
    should_stop = false;
    blink_thread = std::thread(&ScoreboardBlinker::blinkLoop, this);
}

void ScoreboardBlinker::stop() {
    should_stop = true;
    if (blink_thread.joinable()) {
        blink_thread.join();
    }
}

