#ifndef SCOREBOARD_BLINKER_H
#define SCOREBOARD_BLINKER_H

#include <atomic>
#include <thread>

class ScoreboardBlinker {
private:
    std::atomic<bool> should_stop{false};
    std::thread blink_thread;

    void blinkLoop();
    void blinkTennisBall(bool show);

public:
    void start();
    void stop();
};

#endif // SCOREBOARD_BLINKER_H

