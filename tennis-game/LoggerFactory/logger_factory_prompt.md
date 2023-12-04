# Persona
- You are a world-class C++ developer.
- You are an expert at translating JavaScript to C++.
- You are an Object Oriented Programming expert that loves to use Gang of Four design patterns.

# Your task
- Analyze the header files below to understand what has already been implemented.
- Create LoggerFactory .h and .cpp files using the JavaScript LoggerFactory.js provided below as a guide.
- Clone all of the variables and methods provided in the JavaScript LoggerFactory class.
- If you think anything should be added, include it in your answer.

## Javascript LoggerFactory object to use as a guide
```javascript
/** @class LoggerFactory */ 
class LoggerFactory { 
    static FACTORY_ID = "2021";

    static getCookie(name) {
        const value = `; ${document.cookie}`;
        const parts = value.split(`; ${name}=`);
        if (parts.length === 2) return parts.pop().split(';').shift(); }

    static OBJECT_STORAGE_URL = this.getCookie( "OBJECT_STORAGE_URL" ) || "";

    static createLogger( target_object ) {
        if ( this.OBJECT_STORAGE_URL.length == 0 ) { 
            const ConsoleLogger = new Function('return new class ' + target_object + ' { logUpdate( text_to_log ) { console.log( text_to_log ); }}');
            const logger = new ConsoleLogger();
            return logger; }  // if there's no source data set in the cookie, create default and exit.
                              // otherwise, keep going and build the logger...
        const config = {
            runner: "LoggerFactory",
            url:    "https://" + this.OBJECT_STORAGE_URL + "/libraries/local-php-api/index.php/",
            new_id: target_object + "_" + this.FACTORY_ID } // that "_" will indicate that it is from this factory

        const logger = new MonitoredObject( config );
        return logger; }
}
```

## MonitoredObject header file
```cpp
#ifndef MONITOREDOBJECT_H
#define MONITOREDOBJECT_H

#include "../Model/Model.h"
#include "../LogObjectFactory/LogObjectFactory.h"
#include "../MonitorLed/MonitorLed.h"
#include <iostream>
#include <json/json.h> // Include the JSON library
#include <string>
#include <vector>
#include <memory>

class MonitoredObject {
public:
    MonitoredObject(const std::string& new_id);
    void logUpdate(const std::string& message);
    std::string getObjectViewId() const;
    static void processQueryResult(const std::string& results);

private:
    std::string object_view_id;
    std::vector<std::shared_ptr<LogObject>> logObjects;
    std::unique_ptr<Model> model;
    LogObjectFactory logObjectFactory;
    MonitorLed monitorLed;
    std::string data_config;
    
    void initializeModel(const std::string& new_id);
    void updateDataConfig();
};

#endif // MONITOREDOBJECT_H
```

