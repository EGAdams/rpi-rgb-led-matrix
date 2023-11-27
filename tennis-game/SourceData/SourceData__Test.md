# Persona
- Expert C++ Developer
- Expert Object Oriented Programmer
- Gang of Four Design Patterns Expert
- Expert Javascript Developer
- World-class C++ API Developer


# Your Goals
- Create a test For the SourceData Object.
- Make the test very simple.  Here is an example of what a curl call will do:
## Example Test
### A curl call would use this address: https://americansjewelry.com/libraries/local-php-api/index.php/object/select/McbaStartupMonitor_2021

### Expected Result
- The curl call will return the following JSON object:
```json
{"id":399517,"object_view_id":"McbaStartupMonitor_2021","object_data":"{\"logObjects\":[],\"object_view_id\":\"McbaStartupMonitor_2021\",\"objectModel\":{},\"logObjectFactory\":{},\"monitorLed\":{\"config\":{\"new_id\":\"2021\",\"table\":\"monitored_objects\"},\"classObject\":{\"background_color\":\"lightyellow\",\"text_align\":\"left\",\"margin_top\":\"2px\",\"color\":\"black\"},\"ledText\":\"ready.\",\"RUNNING_COLOR\":\"lightyellow\",\"PASS_COLOR\":\"lightgreen\",\"FAIL_COLOR\":\"#fb6666\"}}"}
```

- Use what you know about the SourceData object to make a test that will simulate what the curl call did above.

# Source Code for SourceData.cpp
```cpp
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

void SourceData::selectObject(const std::string& object_view_id, const std::function<void()>& callback) {
    api_path = url + "object/select";
    APIConfigStruct config;
    config.api_path = api_path;
    get(config, [callback](const std::string&) {
        callback();
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
    void get(const APIConfigStruct& config, std::function<void(const std::string&)> callback);
    void post(const std::string& api_path, const std::string& data, const std::function<void()>& callback);
};

#endif // SOURCEDATA_H
```

# Structures to use if needed or implied:
```cpp
#include <string>

struct MonitorLedConfigStruct {
    std::string new_id;
    std::string table;
};

struct MonitorLedClassObjectStruct {
    std::string background_color;
    std::string text_align;
    std::string margin_top;
    std::string color;
};
struct MonitorLedStruct {
    MonitorLedConfigStruct config;
    MonitorLedClassObjectStruct classObject;
    std::string ledText;
    std::string RUNNING_COLOR;
    std::string PASS_COLOR;
    std::string FAIL_COLOR;
};

struct APIConfigStruct {
    std::string api_path;
    std::string type;
    std::string object_view_id;
    MonitorLedStruct monitorLed;
};
```
