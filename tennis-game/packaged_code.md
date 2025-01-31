Please take a loot at the following partial output from the make command:
## The make error
```bash
...
make: *** No rule to make target 'IQueryResultProcessor/IQueryResultProcessor.h', needed by 'run_remote_listener'.
make: *** No rule to make target 'ISourceDataConfig/ISourceDataConfig.h', needed by 'run_remote_listener'.
make: *** No rule to make target 'ISourceQueryConfig/ISourceQueryConfig.h', needed by 'run_remote_listener'.
...
adamsl@DESKTOP-SHDBATI:~/rpi-rgb-led-matrix/tennis-game$
```
Here are the source files that I think are causing the problem.
## MonitoredObject.h
```cpp
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
```

## MonitoredObject.cpp
```cpp
#include "MonitoredObject.h"
#include <iostream>

MonitoredObject::MonitoredObject(const std::string& object_name ) {
    initializeModel( object_name );
}

void MonitoredObject::initializeModel(const std::string& object_name) {
    object_view_id = object_name.find("_") != std::string::npos ? object_name : object_name + "_CPP";
    ISourceDataConfig config; // Create or obtain a suitable ISourceDataConfig object
    //... other initializations for config as needed
    model = std::make_unique<Model>(new SourceData(config));
    monitorLed = MonitorLed();
    updateDataConfig();
    // Rest of the code...
}


void MonitoredObject::logUpdate(const std::string& message) {
    ISourceQueryConfig queryConfig = ISourceQueryConfig();
    IQueryResultProcessor* processor = this;
    if ( object_view_id.empty()) {
        std::cout << "*** ERROR: object needs an id to log. ***" << std::endl;
        return;
    }
    if (message.find("ERROR") != std::string::npos) {
        monitorLed.setFail();
    }
    if (message.find("finished") != std::string::npos) {
        monitorLed.setPass();
    }
    monitorLed.setLedText(message);
    logObjects.push_back(logObjectFactory.createLogObject(message, object_view_id));
    updateDataConfig();
    model->updateObject( queryConfig, processor );
}

std::string MonitoredObject::getObjectViewId() const {
    return object_view_id;
}

void MonitoredObject::processQueryResult( IQueryResultProcessor* iqrp, const std::string& results ) {
    Json::Value root;
    Json::CharReaderBuilder builder;
    std::unique_ptr<Json::CharReader> reader(builder.newCharReader());

    std::string errors;
    bool parsingSuccessful = reader->parse(results.c_str(), results.c_str() + results.size(), &root, &errors);
    if (!parsingSuccessful) {
        // Handle errors
        std::cerr << "Failed to parse JSON: " << errors << std::endl;
        return;
    }

    // Use the 'root' Json::Value as needed
    std::cout << root.toStyledString() << std::endl;
}


void MonitoredObject::updateDataConfig() {
    // Create JSON representation of this object
    Json::Value objJson;
    objJson["object_view_id"] = object_view_id;
    // Populate other fields as needed
    Json::StreamWriterBuilder builder;
    data_config = Json::writeString(builder, objJson);
}
```

## SourceData.h
```cpp
#ifndef SOURCE_DATA_H
#define SOURCE_DATA_H

#include "../ISourceDataConfig.h"
#include "../ISourceQueryConfig.h"
#include "../IQueryResultProcessor.h"

class SourceData {
public:
    SourceData( const ISourceDataConfig& configuration_object );
    ~SourceData();
    void selectAllObjects( IQueryResultProcessor* callbackObject );
    void selectObject( const ISourceQueryConfig& queryConfig, IQueryResultProcessor* callbackObject );
    void insertObject( const ISourceQueryConfig& queryConfig, IQueryResultProcessor* callbackObject );
    void updateObject( const ISourceQueryConfig& queryConfig, IQueryResultProcessor* callbackObject );

private:
    FetchRunner* _runnerObject;
    std::string  _url;
};

#endif // SOURCE_DATA_H
```

## SourceData.cpp
```cpp
#include "SourceData.h"
#include "../IQueryResultProcessor.h"
#include "../FetchRunner/FetchRunner.h"
#include <iostream>
#include "/usr/include/jsoncpp/json/json.h"

SourceData::SourceData(const ISourceDataConfig& configuration_object)
    : _url(configuration_object.url) {

    std::cout << "*** SourceData constructor called. ***" << std::endl;
    std::cout << "url: " << _url << std::endl;

    if (configuration_object.runner != nullptr) {
        _runnerObject = configuration_object.runner;
    } else {
        std::cerr << "Error: Runner object is not initialized." << std::endl;
        // Optionally, create a default runner object or handle the error as needed
        _runnerObject = new FetchRunner(_url); // Example of creating a default FetchRunner
    }
}

SourceData::~SourceData() { delete _runnerObject; }

void SourceData::selectAllObjects( IQueryResultProcessor* callbackObject ) {
    std::string api_path = _url + "object/selectAll";
    FetchRunner runner( api_path );
    std::string run_config = "GET";

    runner.run( run_config, "", [ callbackObject ]( const std::string& result ) {
        callbackObject->processQueryResult( callbackObject, result );
    });
}

void SourceData::selectObject( const ISourceQueryConfig& query_config, IQueryResultProcessor* callbackObject ) {
    std::string config = _url + "object/select/" + query_config.object_view_id;
    std::cout << "config: " << config << std::endl;
    // FetchRunner runner( config );  // If FetchRunner needs to be swapped out...
    std::string run_config = "GET";

    _runnerObject->run( run_config, query_config.object_view_id, [ callbackObject ]( const std::string& result ) {
        callbackObject->processQueryResult( callbackObject, result );
    });
}

void SourceData::insertObject(const ISourceQueryConfig& query_config, IQueryResultProcessor* callbackObject) {
    // Parse the JSON string to extract the name
    Json::Value root;
    Json::Reader reader;
    if (!reader.parse(query_config.object_data, root)) {
        std::cerr << "Error: Failed to parse JSON data in query_config.object_data\n";
        return;
    }
    std::string objectName = root.get("name", "UnnamedObject").asString();

    // Use the extracted name as the object_view_id
    std::string config = _url + objectName;
    std::cout << "config: " << config << std::endl;
    // FetchRunner runner(config);
    std::string run_config = "POST";

    _runnerObject->run( run_config, query_config.object_data, [ callbackObject ]( const std::string& result ) {
        callbackObject->processQueryResult( callbackObject, result );
    });
}


void SourceData::updateObject( const ISourceQueryConfig& query_config, IQueryResultProcessor* callbackObject ) {
    std::string config = _url + "object/update";
    FetchRunner runner( config );
    std::string run_config = "POST";

    runner.run( run_config, query_config.object_data, [ callbackObject ]( const std::string& result ) {
        callbackObject->processQueryResult( callbackObject, result );
    });
}
```

## Model.h
```cpp
#ifndef MODEL_H
#define MODEL_H

#include "../SourceData/SourceData.h"
#include <functional>
#include <string>

class Model {
public:
    explicit Model(SourceData* sourceData) : sourceData(sourceData) {}
    
    void selectObject(const ISourceQueryConfig& queryConfig, IQueryResultProcessor* callbackObject);
    void selectAllObjects(IQueryResultProcessor* callbackObject);
    void insertObject(const ISourceQueryConfig& queryConfig, IQueryResultProcessor* callbackObject);
    void updateObject(const ISourceQueryConfig& queryConfig, IQueryResultProcessor* callbackObject);
    
private:
    SourceData* sourceData;
};
#endif // MODEL_H
```

## Model.cpp
```cpp
#include "Model.h"

void Model::selectObject(const ISourceQueryConfig& queryConfig, IQueryResultProcessor* callbackObject) {
    sourceData->selectObject(queryConfig, callbackObject);
}

void Model::selectAllObjects(IQueryResultProcessor* callbackObject) {
    sourceData->selectAllObjects(callbackObject);
}

void Model::insertObject(const ISourceQueryConfig& queryConfig, IQueryResultProcessor* callbackObject) {
    sourceData->insertObject(queryConfig, callbackObject);
}

void Model::updateObject(const ISourceQueryConfig& queryConfig, IQueryResultProcessor* callbackObject) {
    sourceData->updateObject(queryConfig, callbackObject);
}
```

## Your Task
Please help me fix the make error.