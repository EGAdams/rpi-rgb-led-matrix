#include "SourceData.h"
#include "../IQueryResultProcessor.h"
#include "../FetchRunner/FetchRunner.h"

SourceData::SourceData( const ISourceDataConfig& configuration_object )
    : _runnerObject( configuration_object.runner ), _url( configuration_object.url ) {}
SourceData::~SourceData() { delete _runnerObject; }

void SourceData::selectAllObjects( IQueryResultProcessor* callbackObject ) {
    std::string api_path = _url + "object/selectAll";
    FetchRunner runner( api_path );
    std::string run_config = "GET";

    runner.run( run_config, "", [ callbackObject ]( const std::string& result ) {
        callbackObject->processQueryResult( callbackObject, result );
    });
}

void SourceData::selectObject( const ISourceQueryConfig& query_config, IQueryResultProcessor* callbackObject ) {
    std::string config = _url + "object/select/" + query_config.object_view_id;
    FetchRunner runner( config );  // If FetchRunner needs to be swapped out...
    std::string run_config = "GET";

    runner.run( run_config, query_config.object_view_id, [ callbackObject ]( const std::string& result ) {
        callbackObject->processQueryResult( callbackObject, result );
    });
}

void SourceData::insertObject(const ISourceQueryConfig& query_config, IQueryResultProcessor* callbackObject) {
    std::string config = _url + "object/insert";
    FetchRunner runner(config);
    std::string run_config = "POST";

    runner.run( run_config, query_config.object_data, [ callbackObject ]( const std::string& result ) {
        callbackObject->processQueryResult( callbackObject, result );
    });
}

void SourceData::updateObject( const ISourceQueryConfig& query_config, IQueryResultProcessor* callbackObject ) {
    std::string config = _url + "object/update";
    FetchRunner runner( config );
    std::string run_config = "POST";

    runner.run( run_config, query_config.object_data, [ callbackObject ]( const std::string& result ) {
        callbackObject->processQueryResult( callbackObject, result );
    });
}
