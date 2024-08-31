#ifndef INPUT_WITH_TIMER_H
#define INPUT_WITH_TIMER_H

#include "../ScoreboardBlinker/ScoreboardBlinker.h"
#include "../GameTimer/GameTimer.h"

class InputWithTimer {
private:
    ScoreboardBlinker* _blinker;
    unsigned long _time_slept;

public:
    InputWithTimer( ScoreboardBlinker* blinker );
    ~InputWithTimer();

    int getInput();
    unsigned long getTimeSlept();
};

#endif // INPUT_WITH_TIMER_H