#ifndef MONITOREDOBJECT_H
#define MONITOREDOBJECT_H

#include "../Model/Model.h"
#include "../LogObjectFactory/LogObjectFactory.h"
#include "../MonitorLed/MonitorLed.h"
#include <iostream>
// #include <json/json.h> // Include the JSON library
#include "/usr/include/jsoncpp/json/json.h"
#include <string>
#include <vector>
#include <memory>

class MonitoredObject {
public:
    MonitoredObject(const std::string& new_id);
    void logUpdate(const std::string& message);
    std::string getObjectViewId() const;
    static void processQueryResult(const std::string& results);

private:
    std::string object_view_id;
    std::vector<std::shared_ptr<LogObject>> logObjects;
    std::unique_ptr<Model> model;
    LogObjectFactory logObjectFactory;
    MonitorLed monitorLed;
    std::string data_config;
    
    void initializeModel(const std::string& new_id);
    void updateDataConfig();
};

#endif // MONITOREDOBJECT_H
