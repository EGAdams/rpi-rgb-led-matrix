# Persona
- You are a world-class C++ developer.
- Act as an Expert at translating TypeScript to C++.

# Your task
- Analyze the C++ header files below to understand what has already been implemented.
- Clone all of the variables and methods provided in the TypeScript FetchRunner class.
- Create FetchRunner .h and .cpp files using the TypeScript FetchRunner class provided below as a guide.
- Make sure to play close attention to the runner.run() method in the working SourceData.cpp file.  This is crucial to getting the FetchRunner class to work properly.

## TypeScript FetchRunner class to use as a guide
```typescript
/** @class FetchRunner class */
export default class FetchRunner {
    url                : string;
    url_encoded_header : Record< string, string >;
    json_header        : Record< string, string >;
    fetch_options     !: RequestInit;

    constructor( config: { api_path: string; } ) {
        this.url = config.api_path;
        this.url_encoded_header = { "Content-Type": "application/x-www-form-urlencoded" };
        this.json_header        = { "Content-Type": "application/json"                  };
    } // establish communication address and define header objects.

    async run( apiArgs: { type: string; }, callbackObject : IQueryResultProcessor ): Promise< void > {
        this.fetch_options = {
            method:  apiArgs.type,
            mode:    apiArgs.type === "POST" ? /* POST */ 'no-cors'                 : /* GET */ undefined,
            headers: apiArgs.type === "POST" ? /* POST */ this.json_header          : /* GET */ this.url_encoded_header,
            body:    apiArgs.type === "POST" ? /* POST */ JSON.stringify( apiArgs ) : /* GET */ undefined
        };
        try {
            fetch( this.url, this.fetch_options ).then( res => {
                return res.text();
            }).then( data => {
                callbackObject.processQueryResult( callbackObject, data );
            });
        } catch ( fetch_error ) {
            console.log( fetch_error );
        }
    }
}
```

## SourceData.h
```cpp
class SourceData {
public:
    SourceData(const ISourceDataConfig& configuration_object);
    
    void selectAllObjects(IQueryResultProcessor* callbackObject);
    void selectObject(const ISourceQueryConfig& queryConfig, IQueryResultProcessor* callbackObject);
    void insertObject(const ISourceQueryConfig& queryConfig, IQueryResultProcessor* callbackObject);
    void updateObject(const ISourceQueryConfig& queryConfig, IQueryResultProcessor* callbackObject);

private:
    FetchRunner* runnerObject;
    std::string url;
};
```

## SourceData.cpp
```cpp
#include "SourceData.h"
#include "../IQueryResultProcessor.h"
#include "../FetchRunner/FetchRunner.h"

SourceData::SourceData(const ISourceDataConfig& configuration_object)
    : runnerObject(configuration_object.runner), url(configuration_object.url) {
    // Constructor implementation
}

void SourceData::selectAllObjects(IQueryResultProcessor* callbackObject) {
    std::string api_path = url + "object/selectAll";
    FetchRunner runner(api_path);
    std::string run_config = "GET";

    runner.run(run_config, "", [callbackObject](const std::string& result) {
        callbackObject->processQueryResult(callbackObject, result);
    });
}

void SourceData::selectObject(const ISourceQueryConfig& query_config, IQueryResultProcessor* callbackObject) {
    std::string config = url + "object/select/" + query_config.object_view_id;
    FetchRunner runner(config);
    std::string run_config = "GET";

    runner.run(run_config, query_config.object_view_id, [callbackObject](const std::string& result) {
        callbackObject->processQueryResult(callbackObject, result);
    });
}

void SourceData::insertObject(const ISourceQueryConfig& query_config, IQueryResultProcessor* callbackObject) {
    std::string config = url + "object/insert";
    FetchRunner runner(config);
    std::string run_config = "POST";

    runner.run(run_config, query_config.object_data, [callbackObject](const std::string& result) {
        callbackObject->processQueryResult(callbackObject, result);
    });
}

void SourceData::updateObject(const ISourceQueryConfig& query_config, IQueryResultProcessor* callbackObject) {
    std::string config = url + "object/update";
    FetchRunner runner(config);
    std::string run_config = "POST";

    runner.run(run_config, query_config.object_data, [callbackObject](const std::string& result) {
        callbackObject->processQueryResult(callbackObject, result);
    });
}
```

## IQueryResultProcessor.h
```cpp
class IQueryResultProcessor {
public:
    virtual ~IQueryResultProcessor() = default;
    virtual void processQueryResult(
        IQueryResultProcessor* thisObject, 
        const std::string& queryResultToBeProcessed ) = 0; };
```

## ISourceDataConfig.h
```cpp
// ISourceDataConfig.h
#ifndef ISOURCE_DATA_CONFIG_H
#define ISOURCE_DATA_CONFIG_H
#include "FetchRunner/FetchRunner.h"
#include <string>

struct ISourceDataConfig {
    FetchRunner* runner;
    std::string url;
};
#endif // ISOURCE_DATA_CONFIG_H
```

## ISourceQueryConfig.h
```cpp
// ISourceQueryConfig.h
#ifndef ISOURCE_QUERY_CONFIG_H
#define ISOURCE_QUERY_CONFIG_H

#include <string>

struct ISourceQueryConfig {
    std::string object_view_id; // could be a JSON object, a string, or a custom class
    std::string object_data;    // Replace this with the actual C++ type
};

#endif // ISOURCE_QUERY_CONFIG_H
```
