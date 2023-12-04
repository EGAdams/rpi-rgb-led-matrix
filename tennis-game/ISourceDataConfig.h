// ISourceDataConfig.h
#ifndef ISOURCE_DATA_CONFIG_H
#define ISOURCE_DATA_CONFIG_H
#include "FetchRunner/FetchRunner.h"
#include <string>

struct ISourceDataConfig {
    FetchRunner* runner;
    std::string url;
};
#endif // ISOURCE_DATA_CONFIG_H
