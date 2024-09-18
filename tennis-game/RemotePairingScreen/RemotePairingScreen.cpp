#include "RemotePairingScreen.h"
#include <iostream>

RemotePairingScreen::RemotePairingScreen() 
    : _green_player_paired( false ), _red_player_paired( false ) {}

RemotePairingScreen::~RemotePairingScreen(){}

void RemotePairingScreen::draw() {
    if (!_green_player_paired) {
        std::cout << "     Green Player\n";
        std::cout << "Press your Remote\n";
        std::cout << "     Green button\n\n";
    }

    if (!_red_player_paired) {
        std::cout << "       Red Player\n";
        std::cout << "Press your Remote\n";
        std::cout << "        Red button\n";
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
