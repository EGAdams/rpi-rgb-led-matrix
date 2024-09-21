#include "RemotePairingScreen.h"
#include <iostream>

RemotePairingScreen::RemotePairingScreen( ScoreBoard* scoreboard ) 
    : _green_player_paired( false ), _red_player_paired( false ), _scoreboard( scoreboard ) {}

RemotePairingScreen::~RemotePairingScreen(){}

void RemotePairingScreen::draw() {
    if (!_green_player_paired) {
        if ( _scoreboard->onRaspberryPi()) {
            _scoreboard->clearScreen();
            _scoreboard->writeMessage( "     Green Player\n" );
            _scoreboard->writeMessage( "Press your Remote\n" );
            _scoreboard->writeMessage( "     Green button\n\n" );
        } else {
            std::cout << "     Green Player\n";
            std::cout << "Press your Remote\n";
            std::cout << "     Green button\n\n";  
        }
    }

    if (!_red_player_paired) {
        if( _scoreboard->onRaspberryPi()) {
            _scoreboard->clearScreen();
            _scoreboard->writeMessage( "     Red Player\n" );
            _scoreboard->writeMessage( "Press your Remote\n" );
            _scoreboard->writeMessage( "     Red button\n\n" );
        } else {
            std::cout << "     Red Player\n";
            std::cout << "Press your Remote\n";
            std::cout << "     Red button\n\n";
        }
    }
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
}
