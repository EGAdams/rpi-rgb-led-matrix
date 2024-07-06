#include <iostream>
#include <string>

const std::string bright_green = "\033[92m";  // Bright green
const std::string bright_red = "\033[91m";    // Bright red
const std::string bright_blue = "\033[94m";   // Bright blue
const std::string reset_color = "\033[0m";
const std::string yellow = "\033[93m";

void printFilledLarge1() {
    std::cout << "  __   " << std::endl;
    std::cout << " /_ |  " << std::endl;
    std::cout << "  | |  " << std::endl;
    std::cout << "  | |  " << std::endl;
    std::cout << "  | |  " << std::endl;
    std::cout << "  |_|  " << std::endl;
}

void printFilledLarge0() {
    std::cout << "  ____  " << std::endl;
    std::cout << " / __ \\ " << std::endl;
    std::cout << "| |  | |" << std::endl;
    std::cout << "| |  | |" << std::endl;
    std::cout << "| |__| |" << std::endl;
    std::cout << " \\____/ " << std::endl;
}

void printFilledLarge2() {
    std::cout << "  ___   " << std::endl;
    std::cout << " |__ \\  " << std::endl;
    std::cout << "    ) | " << std::endl;
    std::cout << "   / /  " << std::endl;
    std::cout << "  / /_  " << std::endl;
    std::cout << " |____| " << std::endl;
}

void printFilledLarge10() {
    const std::string yellow = "\033[93m";
    const std::string reset = "\033[0m";
    
    std::cout << yellow;
    std::cout << "  __    ____  " << std::endl;
    std::cout << " /_ |  / __ \\ " << std::endl;
    std::cout << "  | | | |  | |" << std::endl;
    std::cout << "  | | | |  | |" << std::endl;
    std::cout << "  | | | |__| |" << std::endl;
    std::cout << "  |_|  \\____/ " << std::endl;
    std::cout << reset;
}

void printFilledLarge20() {
    const std::string yellow = "\033[93m";
    const std::string reset = "\033[0m";
    
    std::cout << yellow;
    std::cout << "       ___     ____  " << std::endl;
    std::cout << "  _   |__ \\   / __ \\ " << std::endl;
    std::cout << " | |     ) | | |  | |" << std::endl;
    std::cout << " | |    / /  | |  | |" << std::endl;
    std::cout << " |_|   / /_  | |__| |" << std::endl;
    std::cout << "      |____|  \\____/ " << std::endl;
    std::cout << reset;
}

void printFilledLargePipe10() {
    const std::string yellow = "\033[93m";
    const std::string reset = "\033[0m";
    const std::string bright_green = "\033[92m";  // Bright green
    const std::string blue = "\033[94m";  // Blue
    
    std::cout << bright_green;
    std::cout << "         __    ____  " << std::endl;
    std::cout << yellow;
    std::cout << "  _     " << bright_green << "/_ |  ";
    std::cout << bright_green;
    std::cout << "/ __ \\ " << std::endl;
    std::cout << yellow;
    std::cout << " | |     " << bright_green << "| | | ";
    std::cout << bright_green;
    std::cout << "|  | |" << std::endl;
    std::cout << blue;
    std::cout << " | |     | | | ";
    std::cout << bright_green;
    std::cout << "|  | |" << std::endl;
    std::cout << blue;
    std::cout << " |_|     | | | ";
    std::cout << bright_green;
    std::cout << "|__| |" << std::endl;
    std::cout << blue;
    std::cout << "         |_|  ";
    std::cout << bright_green;
    std::cout << "\\____/ " << std::endl;
    std::cout << reset;
}


int main() {
    std::cout << "| \033[92mPLAYER 1: //// \033[92m10 //// \033[92mPLAYER 2: //// \033[92m20 ////" << std::endl;

    // Positioning scores similar to the scoreboard in the video
    std::cout << std::endl << std::endl << std::endl; // Add some space before the scores

    // Player 1's score
    printFilledLargePipe10();

    // Add space between the scores of Player 1 and Player 2
    std::cout << std::endl << std::endl;

    // Player 2's score
    printFilledLarge20();

    return 0;
}
