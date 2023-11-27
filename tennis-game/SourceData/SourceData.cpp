#include "SourceData.h"
#include "../FetchRunner/FetchRunner.h"
#include "../MonitoredObjectStructures.h"
#include <iostream> // For demonstration purposes

SourceData::SourceData(const std::string& url) : url(url) {}

// Function to convert APIConfig to JSON string
std::string toJson(const APIConfigStruct& config) {
    // Implement JSON serialization here
}

void SourceData::selectAllObjects(const std::function<void()>& callback) {
    api_path = url + "object/selectAll";
    APIConfigStruct config;
    config.api_path = api_path;
    get(config, [callback](const std::string&) {
        callback();
    });
}

void SourceData::selectObject(const std::string& object_view_id, const std::function<void(const std::string&)>& callback) {
    api_path = url + "object/select";
    APIConfigStruct config;
    config.api_path = api_path;
    get(config, [callback](const std::string& response) {
        callback(response);
    }); // Pass object_view_id in a real implementation
}

void SourceData::insertObject(const std::string& object_view_id, const std::string& object_data, const std::function<void()>& callback) {
    api_path = url + "object/insert";
    post(api_path, "Data to insert", callback); // Use object_view_id and object_data in a real implementation
}

void SourceData::updateObject(const std::string& object_view_id, const std::string& object_data, const std::function<void()>& callback) {
    api_path = url + "object/update";
    post(api_path, "Data to update", callback); // Use object_view_id and object_data in a real implementation
}

void SourceData::get(const APIConfigStruct& config, std::function<void(const std::string&)> callback) {
    std::string apiPath = url + config.api_path;
    std::string jsonConfig = toJson(config);

    FetchRunner runner(apiPath);
    runner.run("GET", jsonConfig, [callback](const std::string& response) {
        callback(response);
    });
}

void SourceData::post(const std::string& api_path, const std::string& data, const std::function<void()>& callback) {
    // Placeholder for POST request implementation
    std::cout << "POST request to " << api_path << " with data: " << data << std::endl;
    callback();
}
