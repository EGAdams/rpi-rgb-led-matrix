#ifndef SOURCE_DATA_H
#define SOURCE_DATA_H

#include "../ISourceDataConfig.h"
#include "../ISourceQueryConfig.h"
#include "../IQueryResultProcessor.h"

class SourceData {
public:
    SourceData(const ISourceDataConfig& config);
    
    void selectAllObjects(IQueryResultProcessor* callbackObject);
    void selectObject(const ISourceQueryConfig& queryConfig, IQueryResultProcessor* callbackObject);
    void insertObject(const ISourceQueryConfig& queryConfig, IQueryResultProcessor* callbackObject);
    void updateObject(const ISourceQueryConfig& queryConfig, IQueryResultProcessor* callbackObject);

private:
    std::string runnerObject;  // Replace RunnerType with the actual C++ type
    std::string url;
};

#endif // SOURCE_DATA_H