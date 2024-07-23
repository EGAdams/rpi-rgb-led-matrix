#include <iostream>
#include <string>

void printFilledLargePipe10() {
    const std::string yellow = "\033[93m";
    const std::string reset = "\033[0m";
    
    std::cout << yellow;
    std::cout << "         __    ____  " << std::endl;
    std::cout << "  _     /_ |  / __ \\ " << std::endl;
    std::cout << " | |     | | | |  | |" << std::endl;
    std::cout << " | |     | | | |  | |" << std::endl;
    std::cout << " |_|     | | | |__| |" << std::endl;
    std::cout << "         |_|  \\____/ " << std::endl;
    std::cout << reset;
}

int main() {
    std::cout << "| \033[92mPLAYER 1: //// \033[92m10 //// " << std::endl;
    printFilledLargePipe10();
    return 0;
}
