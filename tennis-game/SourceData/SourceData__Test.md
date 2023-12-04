User
Very Good Job.  The test compiled and ran the first time.  Since we know now that the FetchRunner object is working properly, let's make another test C++ file called SourceDataTest.  Let's put a test for this method in this SourceDataTest class: ```cpp void SourceData::selectObject( const ISourceQueryConfig& query_config, IQueryResultProcessor* callbackObject ) {
    std::string config = _url + "object/select/" + query_config.object_view_id;
    FetchRunner runner( config );  // If FetchRunner needs to be swapped out...
    std::string run_config = "GET";

    runner.run( run_config, query_config.object_view_id, [ callbackObject ]( const std::string& result ) {
        callbackObject->processQueryResult( callbackObject, result );
    });
} ```

## g4 answer
https://chat.openai.com/share/fdd1b96d-8142-40f2-af6f-5a51a4b09cd8

