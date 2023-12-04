#include <iostream>
#include <unistd.h> // for sleep()

// Function to draw the scoreboard
void drawScoreboard(int player1Games, int player2Games, int serveBar) {
    // Clear screen and move cursor to the top-left
    std::cout << "\033[2J\033[H";

    // Drawing the scoreboard
    std::cout << "\033[31mPlayer 1: " << player1Games << "\n";
    std::cout << "\033[32mPlayer 2: " << player2Games << "\n";

    // Serve bar
    if (serveBar == 1) {
        std::cout << "\033[34m|\n";
    } else {
        std::cout << "\n";
    }

    // Reset colors
    std::cout << "\033[0m";
}

int main() {
    int player1Games = 0;
    int player2Games = 0;
    int serveBar = 1; // Let's say 1 means Player 1 is serving, 0 means Player 2

    while (true) {
        drawScoreboard(player1Games, player2Games, serveBar);

        // Update game logic here
        // For demonstration, we're just incrementing Player 1's score
        player1Games++;

        // Toggle serve bar for demonstration
        serveBar = 1 - serveBar;

        // Wait for a second
        sleep(1);
    }

    return 0;
}
