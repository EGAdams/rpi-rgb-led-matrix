#ifndef LOGOBJECTCONTAINER_H
#define LOGOBJECTCONTAINER_H

#include "../LogObject/LogObject.h"
#include <vector>
#include <memory>

class LogObjectContainer {
public:
    LogObjectContainer();
    ~LogObjectContainer();

    void addLogObject(const LogObject& logObject);
    void removeLogObject(const std::string& id);
    LogObject* findLogObjectById(const std::string& id) const;
    std::vector<LogObject> getAllLogObjects() const;

private:
    std::vector<std::shared_ptr<LogObject>> logObjects;
};

#endif // LOGOBJECTCONTAINER_H
