#ifndef SOURCE_DATA_H
#define SOURCE_DATA_H

#include "../ISourceDataConfig.h"
#include "../ISourceQueryConfig.h"
#include "../IQueryResultProcessor.h"

class SourceData {
public:
    SourceData( const ISourceDataConfig& configuration_object );
    ~SourceData();
    void selectAllObjects( IQueryResultProcessor* callbackObject );
    void selectObject( const ISourceQueryConfig& queryConfig, IQueryResultProcessor* callbackObject );
    void insertObject( const ISourceQueryConfig& queryConfig, IQueryResultProcessor* callbackObject );
    void updateObject( const ISourceQueryConfig& queryConfig, IQueryResultProcessor* callbackObject );

private:
    FetchRunner* _runnerObject;
    std::string  _url;
};

#endif // SOURCE_DATA_H