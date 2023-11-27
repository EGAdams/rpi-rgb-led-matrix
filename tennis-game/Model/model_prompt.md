# Persona
- You are a world-class C++ developer.
- You are an expert at translating JavaScript to C++.
- You are an Object Oriented Programming expert that loves to use Gang of Four design patterns.

# Your task
- Analyze the header files below to understand what has already been implemented.
- Create Model .h and .cpp files using the JavaScript Model.js provided below as a guide.
- The header and cpp files and dependencies for the SourceData have been provided for you since this class depends heavily on the SourceData class.
- Clone all of the variables and methods provided in the JavaScript Model class.
- If you think anything should be added, include it in your answer.

## Javascript Model object to use as a guide
```javascript
/** @class Model */
/**
 * Creates a new Model instance and hooks up the sourceData.
 *
 * @constructor
 * @param {object} sourceData A reference to the client side sourceData class
 */
 class Model {
    sourceData;
    constructor(sourceData) { this.sourceData = sourceData; }
    /**
         * selects one object from the database
         *
         * @param { function } [ callback ] The callback to fire after the object has been retrieved
         */
    selectObject(data_config, callback) { this.sourceData.selectObject(data_config, callback); }
    /**
         * Gets all objects from the database
         *
         * @param { function } [ callback ] The callback to fire after the objects have been retrieved
         */
    selectAllObjects(callback) { this.sourceData.selectAllObjects(callback); }
    /**
         * Will insert an object into the database.
         *
         * @param {object} data_config The call type, object id and object data
         * @param {function} callback The callback to fire after inserting new data
         */
    insertObject(data_config, callback) { this.sourceData.insertObject(data_config, callback); }
    /**
         * Will update an existing object in the database.
         *
         * @param {object} data_config The call type, object id and object data
         * @param {function} callback The callback to fire after the update
         */
    updateObject(data_config, callback) { this.sourceData.updateObject(data_config, callback); }
}
```

## C++ SourceData.h
```cpp

# Header file for the class: SourceData
```cpp
#ifndef SOURCEDATA_H
#define SOURCEDATA_H

#include <string>
#include <functional>
#include "../FetchRunner/FetchRunner.h"
#include "../MonitoredObjectStructures.h"

class SourceData {
public:
    explicit SourceData(const std::string& url);
    
    void selectAllObjects(const std::function<void()>& callback);
    void selectObject(const std::string& object_view_id, const std::function<void(const std::string&)>& callback);
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

# Cpp file for the class: SourceData
```cpp
#include "SourceData.h"
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

```

# Header files for the class: SourceData


    
### Header file: ../FetchRunner/FetchRunner.h    
```cpp
#ifndef FETCHRUNNER_H
#define FETCHRUNNER_H

#include <string>
#include <functional>
#include <curl/curl.h>

class FetchRunner {
public:
    FetchRunner(const std::string& config);
    ~FetchRunner();

    void run(const std::string& apiArgsType, const std::string& apiArgs, std::function<void(const std::string&)> callback);

private:
    std::string url;
    struct curl_slist* url_encoded_header;
    struct curl_slist* json_header;
    CURL* curl;

    void initializeCurl();
    void setupHeaders();
    static size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp);
};

#endif // FETCHRUNNER_H
```


### Header file: ../MonitoredObjectStructures.h    
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