#ifndef LOGGERFACTORY_H
#define LOGGERFACTORY_H

#include "../MonitoredObject/MonitoredObject.h"
#include <string>
#include <memory>

class LoggerFactory {
public:
    static std::string FACTORY_ID;
    static std::string OBJECT_STORAGE_URL;

    static std::unique_ptr<MonitoredObject> createLogger(const std::string& logger_name );

private:
    LoggerFactory();  // Private to prevent instantiation
};

#endif // LOGGERFACTORY_H
