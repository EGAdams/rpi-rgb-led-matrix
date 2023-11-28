#include "SourceData.h"
#include "../FetchRunner/FetchRunner.h"
#include "../IQueryResultProcessor.h" // Include the interface
#include <iostream>

SourceData::SourceData(const std::string& url) : url(url) {}

void SourceData::selectAllObjects(IQueryResultProcessor* callbackObject) {
    std::string apiPath = url + "/object/selectAll";
    FetchRunner runner(apiPath);

    runner.run("GET", "", [callbackObject](const std::string& response) {
        callbackObject->processQueryResult(callbackObject, response);
    });
}

void SourceData::selectObject(const std::string& object_view_id, IQueryResultProcessor* callbackObject) {
    std::string apiPath = url + "/object/select/" + object_view_id;
    FetchRunner runner(apiPath);

    runner.run("GET", "", [callbackObject](const std::string& response) {
        callbackObject->processQueryResult(callbackObject, response);
    });
}

void SourceData::insertObject(const std::string& object_view_id, const std::string& object_data, IQueryResultProcessor* callbackObject) {
    std::string apiPath = url + "/object/insert";
    FetchRunner runner(apiPath);

    runner.run("POST", object_data, [callbackObject](const std::string& response) {
        callbackObject->processQueryResult(callbackObject, response);
    });
}

void SourceData::updateObject(const std::string& object_view_id, const std::string& object_data, IQueryResultProcessor* callbackObject) {
    std::string apiPath = url + "/object/update";
    FetchRunner runner(apiPath);

    runner.run("POST", object_data, [callbackObject](const std::string& response) {
        callbackObject->processQueryResult(callbackObject, response);
    });
}
