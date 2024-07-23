#include <iostream>
#include <string>

void printFilledLarge1() {
    const std::string yellow = "\033[93m";
    const std::string reset = "\033[0m";

    std::cout << yellow;
    std::cout << "  __  " << std::endl;
    std::cout << " /_ | " << std::endl;
    std::cout << "  | | " << std::endl;
    std::cout << "  | | " << std::endl;
    std::cout << "  | | " << std::endl;
    std::cout << "  |_| " << std::endl;
    std::cout << reset;
}

void printFilledLarge0() {
    const std::string yellow = "\033[93m";
    const std::string reset = "\033[0m";

    std::cout << yellow;
    std::cout << "  ____  " << std::endl;
    std::cout << " / __ \\ " << std::endl;
    std::cout << "| |  | |" << std::endl;
    std::cout << "| |  | |" << std::endl;
    std::cout << "| |__| |" << std::endl;
    std::cout << " \\____/ " << std::endl;
    std::cout << reset;
}

void printFilledLarge2() {
    const std::string yellow = "\033[93m";
    const std::string reset = "\033[0m";

    std::cout << yellow;
    std::cout << "  ___  " << std::endl;
    std::cout << " |__ \\ " << std::endl;
    std::cout << "    ) |" << std::endl;
    std::cout << "   / / " << std::endl;
    std::cout << "  / /_ " << std::endl;
    std::cout << " |____|" << std::endl;
    std::cout << reset;
}

void printFilledLarge10() {
    printFilledLarge1();
    std::cout << std::endl;
    printFilledLarge0();
}

void printFilledLarge20() {
    printFilledLarge2();
    std::cout << std::endl;
    printFilledLarge0();
}

int main() {
    std::cout << "| \033[92mPLAYER 1: //// \033[92m10 //// \033[92mPLAYER 2: //// \033[92m20 ////" << std::endl;

    // Positioning scores similar to the scoreboard in the video
    std::cout << std::endl << std::endl << std::endl; // Add some space before the scores

    // Player 1's score
    printFilledLarge10();

    // Add space between the scores of Player 1 and Player 2
    std::cout << std::endl << std::endl;

    // Player 2's score
    printFilledLarge20();

    return 0;
}
