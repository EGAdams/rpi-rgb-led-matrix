# Persona
- Remember that you are a world-class C++ developer.
- You are an expert at creating mermaid diagrams.

# Your task
- Analyze the header files below to understand what has already been implemented.
- Create FetchRunner .h and .cpp files using the JavaScript FetchRunner.js provided below as a guide.
- Clone all of the variables and methods provided in the JavaScript FetchRunner class.
- If you think anything should be added, include it in your answer.

## Javascript FetchRunner object to use as a guide
```javascript
/** @class FetchRunner class */
class FetchRunner {
    url;
    url_encoded_header;
    json_header;
    fetch_options;
    constructor( config ) {
        this.url = config.api_path;
        this.url_encoded_header = { "Content-Type": "application/x-www-form-urlencoded" };
        this.json_header = { "Content-Type": "application/json" };
    } // establish communication address
    async run ( apiArgs ) {
        this.fetch_options = {
            method: apiArgs.type,
            mode: 'no-cors',
            headers: apiArgs.type == "POST" ? /* POST */ this.json_header : /* GET */ this.url_encoded_header,
            body:    apiArgs.type == "POST" ? /* POST */ JSON.stringify( apiArgs ) : /* GET */ undefined
        };
        fetch( this.url, this.fetch_options ).then( res => {
            // console.log( "processing response: " + res + "..." );
            return res.text();
        }).then( data => {
            // console.log( "data: " + data );
        });
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

Remember to act as a world-class C++ and Javascipt developer.  You are an expert at translating JavaScript to C++.

Please translate the following JavaScript FetchRunner method to it's equivalent C++ code.
```javascript
async run ( apiArgs ) {
        this.fetch_options = {
            method: apiArgs.type,
            mode: 'no-cors',
            headers: apiArgs.type == "POST" ? /* POST */ this.json_header : /* GET */ this.url_encoded_header,
            body:    apiArgs.type == "POST" ? /* POST */ JSON.stringify( apiArgs ) : /* GET */ undefined
        };
        fetch( this.url, this.fetch_options ).then( res => {
            // console.log( "processing response: " + res + "..." );
            return res.text();
        }).then( data => {
            // console.log( "data: " + data );
        });
    }
```