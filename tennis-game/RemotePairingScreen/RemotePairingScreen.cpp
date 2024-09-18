#include "RemotePairingScreen.h"
#include <iostream>

RemotePairingScreen::RemotePairingScreen() 
    : greenPlayerPaired(false), redPlayerPaired(false) {}

void RemotePairingScreen::draw() {
    if (!greenPlayerPaired) {
        std::cout << "     Green Player\n";
        std::cout << "Press your Remote\n";
        std::cout << "     Green button\n\n";
    }

    if (!redPlayerPaired) {
        std::cout << "       Red Player\n";
        std::cout << "Press your Remote\n";
        std::cout << "        Red button\n";
    }
}

void RemotePairingScreen::greenPlayerPressed() {
    greenPlayerPaired = true;
    clearGreenPlayerText();
    if (redPlayerPaired) {
        clearAllText();
    }
}

void RemotePairingScreen::redPlayerPressed() {
    redPlayerPaired = true;
    clearRedPlayerText();
    if (greenPlayerPaired) {
        clearAllText();
    }
}

bool RemotePairingScreen::inPairingMode() const {
    return !(greenPlayerPaired && redPlayerPaired);
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
