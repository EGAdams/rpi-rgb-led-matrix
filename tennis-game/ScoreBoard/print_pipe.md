Here is how a "10" is printed:
```cpp
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
```

Please write the code to print "| 10"