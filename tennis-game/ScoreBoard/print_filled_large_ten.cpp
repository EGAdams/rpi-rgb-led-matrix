#include <iostream>
#include <string>

void printFilledLarge10() {
    const std::string yellow = "\033[93m";
    const std::string reset = "\033[0m";

    std::cout << yellow;
    std::cout << "  ____   " << "  _  " << std::endl;
    std::cout << " / ___|  " << " / | " << std::endl;
    std::cout << "| |  _   " << "| | " << std::endl;
    std::cout << "| |_| |  " << "| | " << std::endl;
    std::cout << " \\____|  " << "|_| " << std::endl;
    std::cout << reset;
}

int main() {
    std::cout << "| \033[92mPLAYER 1: //// \033[92m10 //// " << std::endl;
    printFilledLarge10();
    return 0;
}
