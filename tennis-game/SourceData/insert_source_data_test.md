# Persona
- World-class C++ developer
- Expert C++ API developer

# Goal
- In the below test, the select operation is being checked.  Before this can work properly, we need to get the insert operation working.  Please rewrite the below code to test the insert operation instead of the select operation.

# C++ Source Code
```cpp
#include "SourceData.h"
#include "../IQueryResultProcessor.h"
#include <iostream>
#include <string>

// Mock implementation of IQueryResultProcessor for testing purposes
class MockQueryResultProcessor : public IQueryResultProcessor {
public:
    void processQueryResult( IQueryResultProcessor* thisObject, const std::string& queryResultToBeProcessed ) override {
        std::cout << "Processed Query Result: " << queryResultToBeProcessed << std::endl;
    }
};

void testSelectObject() {
    std::cout << "creating a mock query configuration..." << std::endl;
    ISourceQueryConfig queryConfig; // Create a mock query configuration

    std::cout << "setting the query configuration's object_view_id..." << std::endl;
    queryConfig.object_view_id = "McbaStartupMonitor_2021"; // Assuming this is a valid ID for the test

    std::cout << "creating a source data configuration..." << std::endl;
    ISourceDataConfig sourceConfig; // Create a SourceData instance with a valid URL

    std::cout << "setting the source data configuration's url..." << std::endl;
    sourceConfig.url = "https://americansjewelry.com/libraries/local-php-api/index.php/object/select/";

    std::cout << "creating a source data instance..." << std::endl;
    SourceData sourceData( sourceConfig );

    std::cout << "creating a mock query result processor..." << std::endl;
    MockQueryResultProcessor mockProcessor; // Create a MockQueryResultProcessor instance

    std::cout << "calling the selectObject method..." << std::endl;
    sourceData.selectObject( queryConfig, &mockProcessor ); // Test the selectObject method
}

int main() {
    std::cout << "SourceData::selectObject test started." << std::endl;
    testSelectObject();
    std::cout << "SourceData::selectObject test completed." << std::endl;
    return 0; }
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

SourceData::SourceData( const ISourceDataConfig& configuration_object )
    : _runnerObject( configuration_object.runner ), _url( configuration_object.url ) {}
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
    std::string config = _url + "object/insert";
    FetchRunner runner(config);
    std::string run_config = "POST";

    runner.run( run_config, query_config.object_data, [ callbackObject ]( const std::string& result ) {
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
Here is the command that I use to compile the other code for select: 
```bash
g++ -g -o ./SourceData/source_data_test ./SourceData/SourceDataTest.cpp ./SourceData/SourceData.cpp ./FetchRunner/FetchRunner.cpp -lcurl -ljsoncpp
```
Please write a command that will compile the code for insert.

In this code:
```cpp
SourceData::SourceData( const ISourceDataConfig& configuration_object )
    : _runnerObject( configuration_object.runner ), _url( configuration_object.url ) {
        std::cout << "*** SourceData constructor called. ***" << std::endl;
        std::cout << "url: " << _url << std::endl;
    }
```

I don't think that the runner object is called.  This is causing a segmentation fault because url is not defined in the FetchRunner object.  Please fix this.