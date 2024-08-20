#include "WatchTimer.h"

WatchTimer::WatchTimer() {}
WatchTimer::~WatchTimer() {}

int WatchTimer::watchInputDelay( int delay, Inputs* input, int watchInterval  ) {
    int initialRotaryValue = input->readRotary();
    int totalDelay = 0;
    while ( totalDelay < delay ) {
        GameTimer::gameDelay( watchInterval );
        if ( input->readRotary() != initialRotaryValue ) { return 1; }
        totalDelay += watchInterval;
    }
    return 0; }

int WatchTimer::watchPLayerButtonDelay( int delay, int watchInterval ) {
    int playerButton = 0;
    int totalDelay = 0;
    while ( totalDelay < delay ) {
        GameTimer::gameDelay( watchInterval );
        // get player button here
        if ( playerButton != 0 ) { return 1; }
        totalDelay += watchInterval;
    }
    return 0; }
