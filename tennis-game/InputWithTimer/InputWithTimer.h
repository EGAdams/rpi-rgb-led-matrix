#ifndef INPUT_WITH_TIMER_H
#define INPUT_WITH_TIMER_H

#include "../ScoreboardBlinker/ScoreboardBlinker.h"

class InputWithTimer {
private:
    ScoreboardBlinker* _blinker;

public:
    InputWithTimer( ScoreboardBlinker* blinker );
    ~InputWithTimer();

    int getInput();
};

#endif // INPUT_WITH_TIMER_H