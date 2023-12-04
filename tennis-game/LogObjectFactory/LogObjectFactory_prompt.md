# Persona
- Remember that you are a world-class C++ developer.

# Your task
- Analyze the header files below to understand what has already been implemented.
- Create LogObjectFactory .h and .cpp files using the JavaScript LogObjectFactory.js as a guide.
- If you think anything should be added, please do so.

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

Please begin