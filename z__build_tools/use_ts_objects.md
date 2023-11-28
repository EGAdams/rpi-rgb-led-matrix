I have already created a system like this with Typescript Objects.  Here is a list of them if it will help in the design of this system.

# Typescript Interfaces
```typescript
/**
 *
 * @description
 * Objects that represent finished queries are sent
 * to this type of object for further processing.
 *
 * @interface IQueryResultProcessor
 */
 export default interface IQueryResultProcessor {
    processQueryResult( thisObject: IQueryResultProcessor, queryResultToBeProcessed: any ): void;
}

/** @interface ISourceDataConfig */
interface ISourceDataConfig {
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    Runner : any;
    url    : string;
  }

  export default ISourceDataConfig;

/** @interface ISourceQueryConfig */
interface ISourceQueryConfig {
    object_view_id : string;
    object_data    : any;
  }

  export default ISourceQueryConfig;
```

# Source Data.ts
```typescript
import IQueryResultProcessor from './IQueryResultProcessor';
import ISourceDataConfig from './ISourceDataConfig';
import ISourceQueryConfig from './ISourceQueryConfig';
/**
 *
 * @param {object} configuration_object {
 *     runner: xhr, fetch, or some other object that makes AJAX calls
 *     url   : the place where the runner is going to make the api call
 * }
 * The call type, object id and object data
 * @param {function} callbackObject
 *
 */
export default class SourceData {
    RunnerObject: any;
    url:          string;
    constructor( configuration_object: ISourceDataConfig ) {
        this.RunnerObject = configuration_object.Runner;
        this.url          = configuration_object.url; }

    /**
     * selects all objects from the database
     * @param {function} callbackObject The callbackObject to fire upon retrieving data
     */
    selectAllObjects( callbackObject: IQueryResultProcessor ) {
        const api_path = this.url + "object/selectAll";
        const runner = new this.RunnerObject( api_path );
        const run_config = { type: "GET" }
        runner.run( run_config, callbackObject ); }

    /**
     * selects one object from the database
     * @param {function} callbackObject The callbackObject to fire upon retrieving data
     */
    selectObject( query_config: ISourceQueryConfig, callbackObject: IQueryResultProcessor ) {
        const config = { api_path: `${ this.url }object/select/${ query_config.object_view_id }` };
        const runner = new this.RunnerObject( config );
        const run_config = { type: "GET", object_view_id: query_config.object_view_id }
        runner.run( run_config, callbackObject ); }

    /**
     * Will insert an object into the database.
     * @param {object} query_config The call type, object id and object data
     * @param {function} callbackObject The callbackObject to fire after inserting new data
     */
    insertObject( query_config: ISourceQueryConfig, callbackObject: IQueryResultProcessor ) {
        const config = { api_path: this.url + "object/insert" };
        const runner = new this.RunnerObject( config );
        const run_config = { type: "POST",
                            object_view_id: query_config.object_view_id,
                            object_data:    query_config.object_data };
        runner.run( run_config, callbackObject ); }

    /**
     * Will update an existing object in the database.
     *
     * @param {object} query_config The call type, object id and object data
     * @param {function} callbackObject The callbackObject to fire after the update
     *
     * @example
     * updateObject( { "object_view_id": "Parser_5", "object_data": { "led_color": "red"}}, callbackObject.processQueryResult ) {
     *     // will update object with object_view_id Parser_5's led_color to red.
     * });
     */
    updateObject( query_config: ISourceQueryConfig, callbackObject: IQueryResultProcessor ) {
        const config = { api_path: this.url + "object/update" };
        const runner = new this.RunnerObject( config );
        const run_config = { type: "POST",
                            object_view_id: query_config.object_view_id,
                            object_data:    query_config.object_data };
        runner.run( run_config, callbackObject ); }
}
```

# Model.ts
```typescript
import IQueryResultProcessor from './IQueryResultProcessor';
import ISourceQueryConfig from "./ISourceQueryConfig";
import SourceData from "./SourceData";

/** @class Model */
/**
 * Creates a new Model instance and hooks up the sourceData.
 *
 * @constructor
 * @param {object} sourceData A reference to the client side sourceData class
 */
export default class Model {
    sourceData: SourceData;
    constructor( sourceData: SourceData ) { this.sourceData = sourceData; }
    /**
     * selects one object from the database
     *
     * @param { function } [ callbackObject ] The callbackObject to fire after the object has been retrieved
     */
    selectObject ( query_config: ISourceQueryConfig, callbackObject: IQueryResultProcessor ) {
        this.sourceData.selectObject( query_config, callbackObject ); }
    /**
     * Gets all objects from the database
     *
     * @param { function } [ callbackObject ] The callbackObject to fire after the objects have been retrieved
     */
    selectAllObjects ( callbackObject: IQueryResultProcessor ) { this.sourceData.selectAllObjects( callbackObject ); }
    /**
     * Will insert an object into the database.
     *
     * @param {object} query_config The call type, object id and object data
     * @param {function} callbackObject The callbackObject to fire after inserting new data
     */
    insertObject ( query_config: ISourceQueryConfig, callbackObject: IQueryResultProcessor ) { this.sourceData.insertObject( query_config, callbackObject ); }
    /**
     * Will update an existing object in the database.
     *
     * @param {object} query_config The call type, object id and object data
     * @param {function} callbackObject The callbackObject to fire after the update
     */
    updateObject ( query_config: ISourceQueryConfig, callbackObject: IQueryResultProcessor ) { this.sourceData.updateObject( query_config, callbackObject ); }
}
```

## C++ FetchRunner Object
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

These objects are proven to work and are in production.  I am not sure if they will help in the design of this system, but I thought I would share them just in case.  Please use this design if it will help.
Please help me make more C++ Objects if it would help in the design of this system.  The FetchRunner object is the only one I have made so far.


