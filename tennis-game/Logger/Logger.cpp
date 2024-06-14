#include "Logger.h"
#include <algorithm>
#include <array>
#include <ctime>
#include <iostream>
#include <regex>

Logger::Logger( std::string name ) : _name( name ) {
    _log_file.open(name + "/log.txt", std::ios::out | std::ios::app);
    if (!_log_file.is_open()) { // Handle error
        std::cerr << "Failed to open log file" << std::endl; }}

Logger::~Logger() { _log_file.close(); }

std::string Logger::getName() { return _name; }

void Logger::setName(std::string name) { _name = name; }

void Logger::logUpdate(std::string message) {
    if (!_log_file.is_open()) {
        std::cerr << "Log file not open for message [" << message << "]" << std::endl;
        return;
    }
    message = std::regex_replace(message, std::regex(":"), "\\:");
    int random_variable = this->_get_random_number();
    _log_file << "{\"timestamp\":"
        << this->_get_seconds_since_epoch() * 1000;  // timestamp
    _log_file << ",\"id\":\"" << _name << "_" << random_variable << "_"
        << this->_get_seconds_since_epoch() * 1000;  // id
    _log_file << "\",\"message\":\"" << message;           // message
    _log_file << "\",\"method\":\"" << this->_name << "\"}";    // method
    _log_file << std::endl;
}

decltype( std::chrono::seconds().count() ) Logger::_get_seconds_since_epoch() {
    const auto now = std::chrono::system_clock::now();
    const auto epoch = now.time_since_epoch();
    const auto seconds = std::chrono::duration_cast< std::chrono::seconds >( epoch );
    return seconds.count();
}

int Logger::_get_random_number() {
    time_t now = time( 0 );
    std::srand( std::time( &now ) );  // use current time as seed for random generator
    GameTimer::gameDelay( 100 );
    int random_variable = std::rand();
    while ( inArray( random_variable ) ) {
        random_variable++;
    }
    _used_random_numbers.push_back( random_variable );
    return random_variable;
}

bool Logger::inArray( int supposed_random_number ) {
    if ( std::count( _used_random_numbers.begin(),
        _used_random_numbers.end(),
        supposed_random_number ) > 0 ) {
        return true;
    }
    return false; }
