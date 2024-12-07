Here is the C++ code that is calling the remote pairing instructions:
```c++
while ( remotePairingScreen.inPairingMode()) {
    remotePairingScreen.draw();
    std::cin >> menu_selection;
    if ( menu_selection == 1 ) {
        remotePairingScreen.greenPlayerPressed();
    } else if ( menu_selection == 2 ) {
        remotePairingScreen.redPlayerPressed();
    } else {
        print( "*** invalid selection during remote pairing. ***" );
        GameTimer::gameDelay( 1000 );
    }
}
```

# Remote Pairing Screen class
```c++
RemotePairingScreen::RemotePairingScreen( ScoreBoard* scoreboard ) 
    : _green_player_paired( false ), _red_player_paired( false ), _scoreboard( scoreboard ) {}

RemotePairingScreen::~RemotePairingScreen(){}

void RemotePairingScreen::draw() {
    if (!_green_player_paired) {
        if ( _scoreboard->onRaspberryPi()) {
            #define LEFT_MARGIN 8
            _scoreboard->clearScreen();
            _scoreboard->drawText( "Green",   GREEN, LEFT_MARGIN + 1, 17  );
            _scoreboard->drawText( "Player",  GREEN, LEFT_MARGIN + 0, 34  );
            _scoreboard->drawText( "Press",   GREEN, LEFT_MARGIN + 1, 51  );
            _scoreboard->drawText( "Your",    GREEN, LEFT_MARGIN + 2, 68  );
            _scoreboard->drawText( "Remote",  GREEN, LEFT_MARGIN + -8, 85 );
            _scoreboard->drawText( "Green",   GREEN, LEFT_MARGIN + 1, 102 );
            _scoreboard->drawText( "Button",  GREEN, LEFT_MARGIN + 0, 119 );
        } else {
            std::cout << "     Green Player\n";
            std::cout << "     Player\n";
            std::cout << "Press your Remote\n";
            std::cout << "     Green button\n\n";  
        }
    }
    GameTimer::gameDelay( 1000 );
    if (!_red_player_paired) {
        if( _scoreboard->onRaspberryPi()) {
            #define LEFT_MARGIN 8
            _scoreboard->clearScreen();
            _scoreboard->drawText( "Red",   RED, LEFT_MARGIN + 4, 17  );
            _scoreboard->drawText( "Player",  RED, LEFT_MARGIN + 0, 34  );
            _scoreboard->drawText( "Press",   RED, LEFT_MARGIN + 1, 51  );
            _scoreboard->drawText( "Your",    RED, LEFT_MARGIN + 2, 68  );
            _scoreboard->drawText( "Remote",  RED, LEFT_MARGIN + -8, 85 );
            _scoreboard->drawText( "Red",   RED, LEFT_MARGIN + 4, 102 );
            _scoreboard->drawText( "Button",  RED, LEFT_MARGIN + 0, 119 );
        } else {
            std::cout << "     Red Player\n";
            std::cout << "Press your Remote\n";
            std::cout << "     Red button\n\n";
        }
    }
    GameTimer::gameDelay( 1000 );
}

void RemotePairingScreen::greenPlayerPressed() {
    _green_player_paired = true;
    clearGreenPlayerText();
    if (_red_player_paired) {
        clearAllText();
    }
}

void RemotePairingScreen::redPlayerPressed() {
    _red_player_paired = true;
    clearRedPlayerText();
    if (_green_player_paired) {
        clearAllText();
    }
}

bool RemotePairingScreen::inPairingMode() const {
    return !(_green_player_paired && _red_player_paired);
}

void RemotePairingScreen::clearGreenPlayerText() {
    std::cout << "Green Player paired, clearing green player text...\n";
}

void RemotePairingScreen::clearRedPlayerText() {
    std::cout << "Red Player paired, clearing red player text...\n";
}

void RemotePairingScreen::clearAllText() {
    std::cout << "Both players paired, starting the game...\n";
    _scoreboard->clearScreen();
    _scoreboard->drawText( "Paired", YELLOW, 5, 65 );
    GameTimer::gameDelay( 2000 );
    _scoreboard->clearScreen();
    _scoreboard->update();
}
```

When this code runs, the Green instructions display, it waits 1 second and then the Red instructions display, it waits 1 second and then the Green instructions display again.

Insead, I want to have the Green instructions display, it waits 1 second and then the Red instructions display, it waits 1 second and then the Green instructions display again and this repeat until the Green and Red players have paired.

Please modify the code to achieve this behavior.

Can you use something similar to this solution? 
```c++
#include "InputWithTimer.h"
#include <iostream>


InputWithTimer::InputWithTimer( ScoreboardBlinker* blinker) : _blinker( blinker ) {}
InputWithTimer::~InputWithTimer() {}

int InputWithTimer::getInput() {
    // Mark start time with game timer
    unsigned long sleep_start = GameTimer::gameMillis();
    int menu_selection;
    _blinker->start();
    std::cin >> menu_selection;
    _blinker->stop();
    // Mark end time with game timer
    unsigned long sleep_end = GameTimer::gameMillis();
    _time_slept = sleep_end - sleep_start;
    return menu_selection;
}

unsigned long InputWithTimer::getTimeSlept() {
    return _time_slept;
}

#include "ScoreboardBlinker.h"
#include <chrono>
#include <thread>

ScoreboardBlinker::ScoreboardBlinker( ScoreBoard* scoreBoard ) :
    _scoreboard( scoreBoard ) {
        _sleep_start = GameTimer::gameMillis();;
    }

ScoreboardBlinker::~ScoreboardBlinker() { stop(); }

void ScoreboardBlinker::blinkLoop() {
    unsigned long time_now     = GameTimer::gameMillis();
    unsigned long time_elapsed = time_now - _sleep_start;
    while ( !should_stop ) {
        blinkTennisBall( true );
        std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
        time_now = GameTimer::gameMillis();
        time_elapsed = (time_now - _sleep_start) / 1000;
        if ( time_elapsed >  MAX_SLEEP ) {
            print( "UNDO (9): Reset Game        Player1 Score (1): Reset Game  Player2 Score (2): Reset Game   slept " << time_elapsed << " seconds." );    
        }
        blinkTennisBall( false );
        std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
        time_now = GameTimer::gameMillis();
        time_elapsed = ( time_now - _sleep_start ) / 1000;
        if ( time_elapsed <= MAX_SLEEP ) {
            print( "UNDO (9): Enter last match  Player1 Score (1): Reset Game  Player2 Score (2): Reset Game   slept " << time_elapsed << " seconds.");    
        }
    }
}

void ScoreboardBlinker::blinkTennisBall( bool show ) {
    // Implement the logic to show/hide the tennis ball on the scoreboard
    // This will depend on your existing scoreboard implementation
    if ( show ) {
        // std::cout << "drawing yellow period... " << std::endl;
        _scoreboard->drawYellowPeriod();
    }
    else {
        _scoreboard->clearScreen();
    }
}

void ScoreboardBlinker::start() {
    should_stop = false;
    blink_thread = std::thread( &ScoreboardBlinker::blinkLoop, this );
}

void ScoreboardBlinker::stop() {
    should_stop = true;
    if ( blink_thread.joinable() ) {
        blink_thread.join();
    }
}
```

