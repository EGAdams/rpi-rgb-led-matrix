#ifndef REMOTE_INPUT_WITH_TIMER_H
#define REMOTE_INPUT_WITH_TIMER_H

#include "../Blinker/Blinker.h"
#include "../GameTimer/GameTimer.h"
#include "../Inputs/Inputs.h"
#include "../IInputWithTimer/IInputWithTimer.h"

class RemoteInputWithTimer : public IInputWithTimer {
private:
    Blinker*        _blinker;
    Inputs*         _inputs;

public:
    RemoteInputWithTimer( Blinker* blinker, Inputs* inputs, unsigned long timeout_ms = 4000 );
    ~RemoteInputWithTimer();

    int getInput();
    unsigned long getTimeSlept();
};

#endif // REMOTE_INPUT_WITH_TIMER_H
