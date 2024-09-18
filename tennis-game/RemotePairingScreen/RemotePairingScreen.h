#ifndef REMOTE_PAIRING_SCREEN_H
#define REMOTE_PAIRING_SCREEN_H

#include <string>

class RemotePairingScreen {
public:
    RemotePairingScreen();

    // Method to display the instructions on the screen
    void draw();

    // Called when the Green Player presses their button
    void greenPlayerPressed();

    // Called when the Red Player presses their button
    void redPlayerPressed();

    // Check if still in pairing mode
    bool inPairingMode() const;

private:
    bool greenPlayerPaired;
    bool redPlayerPaired;

    // Helper to clear the screen or specific text area
    void clearGreenPlayerText();
    void clearRedPlayerText();
    void clearAllText();
};

#endif // REMOTE_PAIRING_SCREEN_H
