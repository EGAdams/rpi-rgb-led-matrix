#ifndef INPUT_WITH_TIMER_H
#define INPUT_WITH_TIMER_H

#include "../ScoreboardBlinker/ScoreboardBlinker.h"

class InputWithTimer {
private:
    ScoreboardBlinker blinker;

public:
    int getInput();
};

#endif // INPUT_WITH_TIMER_H