#ifndef MONITOREDOBJECT_H
#define MONITOREDOBJECT_H

#include "../Model/Model.h"
#include "../LogObjectFactory/LogObjectFactory.h"
#include "../MonitorLed/MonitorLed.h"
#include <iostream>
#include <jsoncpp/json/json.h>
#include <string>
#include <vector>
#include <memory>

class MonitoredObject : public IQueryResultProcessor {
public:
    MonitoredObject(const std::string& new_id);
    void logUpdate(const std::string& message);
    std::string getObjectViewId() const;
    void processQueryResult( IQueryResultProcessor* processor, const std::string& results ) override;


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
