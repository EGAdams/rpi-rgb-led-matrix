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
