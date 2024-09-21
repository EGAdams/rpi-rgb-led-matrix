#include "RemotePairingScreen.h"
#include <iostream>

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
            _scoreboard->drawText( "Remote",  GREEN, LEFT_MARGIN + -6, 85 );
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
            _scoreboard->drawText( "Red",   GREEN, LEFT_MARGIN + 3, 17  );
            _scoreboard->drawText( "Player",  GREEN, LEFT_MARGIN + 0, 34  );
            _scoreboard->drawText( "Press",   GREEN, LEFT_MARGIN + 1, 51  );
            _scoreboard->drawText( "Your",    GREEN, LEFT_MARGIN + 2, 68  );
            _scoreboard->drawText( "Remote",  GREEN, LEFT_MARGIN + -6, 85 );
            _scoreboard->drawText( "Red",   GREEN, LEFT_MARGIN + 3, 102 );
            _scoreboard->drawText( "Button",  GREEN, LEFT_MARGIN + 0, 119 );
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
