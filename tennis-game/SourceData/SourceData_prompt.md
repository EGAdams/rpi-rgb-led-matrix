# Persona
- Remember that you are a world-class C++ developer.

# Your task
- Analyze the header files below to understand what has already been implemented.
- Create SourceData .h and .cpp files using the JavaScript SourceData.js provided below as a guide.
- Clone all of the variables and methods provided in the JavaScript SourceData class.
- If you think anything should be added, include it in your answer.

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
