# Persona
- Remember that you are a world-class C++ developer.

# Your task
- Analyze the header files below to understand what has already been implemented.
- Create MonitorLed .h and .cpp files using the JavaScript MonitorLed.js provided below as a guide.
- Use all of the variables and methods provided in the MonitorLed class.
- If you think anything should be added, please do so.

## Javascript MonitorLed object to use as a guide
```javascript
class MonitorLed {
    classObject;
    ledText;
    RUNNING_COLOR;
    PASS_COLOR;
    FAIL_COLOR;
    constructor() {
        this.classObject = new MonitorLedClassObject();
        this.ledText = "ready.";
        this.RUNNING_COLOR = "lightyellow";
        this.PASS_COLOR = "lightgreen";
        this.FAIL_COLOR = "#fb6666"; // lightred is not understood by CSS.  Whaaa... ??
    }
    setFail() {
        this.setLedBackgroundColor( this.FAIL_COLOR );
        this.setLedTextColor( "white" );
    }
    setPass() {
        this.setLedBackgroundColor( this.PASS_COLOR );
        this.setLedTextColor( "black" );
    }
    setLedBackgroundColor( newColor ) { this.classObject.background_color = newColor; }
    setLedTextColor( newColor ) { this.classObject.color = newColor; }
    setLedText( newText ) { this.ledText = newText; }
}
```

## MonitorLedClass header file
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
