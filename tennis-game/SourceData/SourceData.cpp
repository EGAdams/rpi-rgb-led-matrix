#include "SourceData.h"
#include <iostream> // For demonstration purposes

SourceData::SourceData(const std::string& url) : url(url) {}

void SourceData::selectAllObjects(const std::function<void()>& callback) {
    api_path = url + "object/selectAll";
    get(api_path, callback);
}

void SourceData::selectObject(const std::string& object_view_id, const std::function<void()>& callback) {
    api_path = url + "object/select";
    get(api_path, callback); // Pass object_view_id in a real implementation
}

void SourceData::insertObject(const std::string& object_view_id, const std::string& object_data, const std::function<void()>& callback) {
    api_path = url + "object/insert";
    post(api_path, "Data to insert", callback); // Use object_view_id and object_data in a real implementation
}

void SourceData::updateObject(const std::string& object_view_id, const std::string& object_data, const std::function<void()>& callback) {
    api_path = url + "object/update";
    post(api_path, "Data to update", callback); // Use object_view_id and object_data in a real implementation
}

void SourceData::get(const std::string& api_path, const std::function<void()>& callback) {
    // Placeholder for GET request implementation
    std::cout << "GET request to " << api_path << std::endl;
    callback();
}

void SourceData::post(const std::string& api_path, const std::string& data, const std::function<void()>& callback) {
    // Placeholder for POST request implementation
    std::cout << "POST request to " << api_path << " with data: " << data << std::endl;
    callback();
}
