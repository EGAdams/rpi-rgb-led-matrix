#ifndef Logger_h
#define Logger_h
#include <chrono>
#include <fstream>
#include <string>
#include <vector>
#include "../GameTimer/GameTimer.h"

class Logger {
public:
    Logger( std::string name );
    ~Logger();
    void logUpdate( std::string message );
    decltype( std::chrono::seconds().count() ) _get_seconds_since_epoch();
    int _get_random_number();
    bool inArray( int supposed_random_number );
    void setName( std::string name );
    std::string getName();

private:
    std::string _name;
    std::ofstream _log_file;
    std::vector<int> _used_random_numbers;
    std::string _log_file_path;
};
#endif  // Logger_h
