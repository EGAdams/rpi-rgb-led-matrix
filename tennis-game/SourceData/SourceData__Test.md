# Persona
- Expert C++ Developer
- Expert Object Oriented Programmer
- Gang of Four Design Patterns Expert
- Expert Javascript Developer
- World-class C++ API Developer


# Your Goal -Create a test For the SourceData Object.
- Make the test very simple.  Here is an example of what the test will do:
## Example Test
### Call https://americansjewelry.com/libraries/local-php-api/index.php/object/select/McbaStartupMonitor_2021

### Expected Result
- The test will return the following JSON object:
```json
{"id":399517,"object_view_id":"McbaStartupMonitor_2021","object_data":"{\"logObjects\":[],\"object_view_id\":\"McbaStartupMonitor_2021\",\"objectModel\":{},\"logObjectFactory\":{},\"monitorLed\":{\"config\":{\"new_id\":\"2021\",\"table\":\"monitored_objects\"},\"classObject\":{\"background_color\":\"lightyellow\",\"text_align\":\"left\",\"margin_top\":\"2px\",\"color\":\"black\"},\"ledText\":\"ready.\",\"RUNNING_COLOR\":\"lightyellow\",\"PASS_COLOR\":\"lightgreen\",\"FAIL_COLOR\":\"#fb6666\"}}"}
```

# Source Code for SourceData.cpp
```cpp
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
```

# Source Code for SourceData.h
```cpp
#ifndef SOURCEDATA_H
#define SOURCEDATA_H

#include <string>
#include <functional>

class SourceData {
public:
    explicit SourceData(const std::string& url);
    
    void selectAllObjects(const std::function<void()>& callback);
    void selectObject(const std::string& object_view_id, const std::function<void()>& callback);
    void insertObject(const std::string& object_view_id, const std::string& object_data, const std::function<void()>& callback);
    void updateObject(const std::string& object_view_id, const std::string& object_data, const std::function<void()>& callback);

private:
    std::string url;
    std::string api_path;

    // Helper methods for making HTTP requests
    void get(const std::string& api_path, const std::function<void()>& callback);
    void post(const std::string& api_path, const std::string& data, const std::function<void()>& callback);
};

#endif // SOURCEDATA_H

```