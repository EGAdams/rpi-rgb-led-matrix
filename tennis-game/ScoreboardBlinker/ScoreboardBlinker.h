#ifndef SCOREBOARD_BLINKER_H
#define SCOREBOARD_BLINKER_H

#include <atomic>
#include <thread>
#include "../ScoreBoard/ScoreBoard.h"

class ScoreboardBlinker {
private:
    std::atomic<bool> should_stop{false};
    std::thread blink_thread;
    ScoreBoard* _scoreboard;

    void blinkLoop();
    void blinkTennisBall(bool show);

public:
    ScoreboardBlinker( ScoreBoard* scoreBoard );
    ~ScoreboardBlinker();
    void start();
    void stop();
};

#endif // SCOREBOARD_BLINKER_H

