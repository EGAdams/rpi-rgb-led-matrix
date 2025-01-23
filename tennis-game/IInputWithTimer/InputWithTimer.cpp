#include "IInputWithTimer.h"

IInputWithTimer::IInputWithTimer(Blinker* blinker, unsigned long timeout_ms)
    : _blinker(blinker), _timeout_ms(timeout_ms), _time_slept(0) {}

unsigned long IInputWithTimer::getTimeSlept() const {
    return _time_slept;
}

void IInputWithTimer::setTimeout( unsigned long timeout ) {
    _timeout_ms = timeout;
}
