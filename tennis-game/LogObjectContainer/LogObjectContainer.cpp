#include "LogObjectContainer.h"
#include <algorithm>

LogObjectContainer::LogObjectContainer() {}

LogObjectContainer::~LogObjectContainer() {}

void LogObjectContainer::addLogObject(const LogObject& logObject) {
    logObjects.push_back(std::make_shared<LogObject>(logObject));
}

void LogObjectContainer::removeLogObject(const std::string& id) {
    logObjects.erase(std::remove_if(logObjects.begin(), logObjects.end(),
                                    [&id](const std::shared_ptr<LogObject>& obj) { return obj->getId() == id; }),
                     logObjects.end());
}

LogObject* LogObjectContainer::findLogObjectById(const std::string& id) const {
    auto it = std::find_if(logObjects.begin(), logObjects.end(),
                           [&id](const std::shared_ptr<LogObject>& obj) { return obj->getId() == id; });
    if (it != logObjects.end()) {
        return it->get();
    }
    return nullptr;
}

std::vector<LogObject> LogObjectContainer::getAllLogObjects() const {
    std::vector<LogObject> allObjects;
    for (const auto& obj : logObjects) {
        allObjects.push_back(*obj);
    }
    return allObjects;
}
