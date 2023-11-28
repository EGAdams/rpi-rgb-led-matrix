// ISourceDataConfig.h
#ifndef ISOURCE_DATA_CONFIG_H
#define ISOURCE_DATA_CONFIG_H

#include <string>

struct ISourceDataConfig {
    // Assuming `Runner` is a type that performs HTTP requests in C++ // changed Runner to string
    std::string  runner; // Replace RunnerType with the actual C++ type
    std::string url;
};

#endif // ISOURCE_DATA_CONFIG_H
