#ifndef LOG_OBJECT_FACTORY_H
#define LOG_OBJECT_FACTORY_H

#include "../LogObject/LogObject.h"
#include <string>
#include <memory>

class LogObjectFactory {
public:
    LogObjectFactory();
    ~LogObjectFactory();

    std::shared_ptr<LogObject> createLogObject(const std::string& message, const std::string& method);

private:
    std::string generateUniqueId();
    uint64_t getCurrentTimestamp();
};
#endif // LOG_OBJECT_FACTORY_H
