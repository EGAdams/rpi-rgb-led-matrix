# Persona
- Remember that you are a world-class C++ developer.

# Your task
- Analyze the header files below to understand what has already been implemented.
- Implement SourceData::get using the JavaScript SourceData.js provided below as a guide.
- Use the FetchRunner class run() method to make the API call.

# JSON config object that will need to be stringified to use in your solution
```json
{
    "api_path": "https://americansjewelry.com/libraries/local-php-api/index.php/object/select/McbaStartupMonitor_2021",
    "type": "GET",
    "object_view_id": "McbaStartupMonitor_2021",
    "object_data": {
        "logObjects": [],
        "object_view_id": "McbaStartupMonitor_2021",
        "objectModel": {},
        "logObjectFactory": {},
        "monitorLed": {
            "config": {
                "new_id": "2021",
                "table": "monitored_objects"
            },
            "classObject": {
                "background_color": "lightyellow",
                "text_align": "left",
                "margin_top": "2px",
                "color": "black"
            },
            "ledText": "ready.",
            "RUNNING_COLOR": "lightyellow",
            "PASS_COLOR": "lightgreen",
            "FAIL_COLOR": "#fb6666"
        }
    }
}
```

## C++ FetchRunner class to use for the SourceData::get method
```cpp
class FetchRunner {
public:
    FetchRunner(const std::string& config);
    ~FetchRunner();
    void run(const std::string& apiArgsType, const std::string& apiArgs);

private:
    std::string url;
    struct curl_slist* url_encoded_header;
    struct curl_slist* json_header;
    CURL* curl;

    void initializeCurl();
    void setupHeaders();
    static size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp);
};
```

## Javascript SourceData object to use as a guide
```javascript
/**
 *
 * @param {object} configuration_object {
 *     runner: xhr, fetch, or some other object that makes AJAX calls
 *     url   : the place where the runner is going to make the api call
 * }
 * The call type, object id and object data
 * @param {function} callback
 *
 */
 class SourceData {
    RunnerObject;
    url;
    api_path;
    constructor(configuration_object) {
        // this.RunnerObject = configuration_object.runner;  won't let me construct for some reason.  worked before ??
        this.url = configuration_object.url;
    }
    /**
     * selects all objects from the database
     * @param {function} callback The callback to fire upon retrieving data
     */
    selectAllObjects(callback) {
        const api_path = this.url + "object/selectAll";
        const runner = new FetchRunner(api_path);
        const run_config = { type: "GET" };
        runner.run(run_config, callback);
    }
    /**
     * selects one object from the database
     * @param {function} callback The callback to fire upon retrieving data
     */
    selectObject(data_config, callback) {
        const config = { api_path: this.url + "object/select" };
        const runner = new FetchRunner(config);
        const run_config = { type: "GET", object_view_id: data_config.object_view_id };
        runner.run(run_config, callback);
    }
    /**
     * Will insert an object into the database.
     * @param {object} data_config The call type, object id and object data
     * @param {function} callback The callback to fire after inserting new data
     */
    insertObject(data_config, callback) {
        const config = { api_path: this.url + "object/insert" };
        const runner = new FetchRunner(config);
        const run_config = { type: "POST",
            object_view_id: data_config.object_view_id,
            object_data: data_config.object_data };
        runner.run(run_config, callback);
    }
    /**
     * Will update an existing object in the database.
     *
     * @param {object} data_config The call type, object id and object data
     * @param {function} callback The callback to fire after the update
     *
     * @example
     * updateObject( { "object_view_id": "Parser_5", "object_data": { "led_color": "red"}}, callback ) {
     *     // will update object with object_view_id Parser_5's led_color to red.
     * });
     */
    updateObject(data_config, callback) {
        const config = { api_path: this.url + "object/update" };
        const runner = new FetchRunner(config);
        const run_config = { type: "POST",
            object_view_id: data_config.object_view_id,
            object_data: data_config.object_data };
        runner.run(run_config, callback);
    }
}
```

## MonitorLed header file
```cpp
class MonitorLed {
public:
    MonitorLed();

    void setFail();
    void setPass();
    void setLedBackgroundColor(const std::string& newColor);
    void setLedTextColor(const std::string& newColor);
    void setLedText(const std::string& newText);

private:
    MonitorLedClassObject classObject;
    std::string ledText;
    static const std::string RUNNING_COLOR;
    static const std::string PASS_COLOR;
    static const std::string FAIL_COLOR;
};
```

## MonitorLedClassObject header file
```cpp
class MonitorLedClassObject {
public:
    MonitorLedClassObject();

    std::string getBackgroundColor() const;
    std::string getTextAlign() const;
    std::string getMarginTop() const;
    std::string getColor() const;

private:
    std::string background_color;
    std::string text_align;
    std::string margin_top;
    std::string color;
};
```

## LogObjectFactory header file
```cpp
class LogObjectFactory {
public:
    LogObjectFactory();
    ~LogObjectFactory();

    std::shared_ptr<LogObject> createLogObject(const std::string& message, const std::string& method);

private:
    std::string generateUniqueId();
    uint64_t getCurrentTimestamp();
};
```

## LogObjectContainer header file
```cpp
#include "../LogObject/LogObject.h"
#include <vector>
#include <memory>

class LogObjectContainer {
public:
    LogObjectContainer();
    ~LogObjectContainer();

    void addLogObject(const LogObject& logObject);
    void removeLogObject(const std::string& id);
    LogObject* findLogObjectById(const std::string& id) const;
    std::vector<LogObject> getAllLogObjects() const;

private:
    std::vector<std::shared_ptr<LogObject>> logObjects;
};
```

## LogObject header file
```cpp
#include <string>
#include <cstdint>

class LogObject {
public:
    LogObject();
    ~LogObject();

    const std::string& getId() const;
    void setId(const std::string& id);

    uint64_t getTimestamp() const;
    void setTimestamp(uint64_t timestamp);

    const std::string& getMessage() const;
    void setMessage(const std::string& message);

    const std::string& getMethod() const;
    void setMethod(const std::string& method);

private:
    std::string id;
    uint64_t timestamp;
    std::string message;
    std::string method;
};
```
`g4_answer`
https://chat.openai.com/share/dacca2b9-a881-4da8-9308-a2fff913cd11
