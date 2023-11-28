#ifndef IQUERY_RESULT_PROCESSOR_H
#define IQUERY_RESULT_PROCESSOR_H

#include <string>

/** 
 *  IQureyResultProcessor is an interface for processing query results.
 *  It is used by SourceData to process query results.
 *  It is implemented by the caller of SourceData.
 */
class IQueryResultProcessor {
public:
    virtual ~IQueryResultProcessor() {}
    virtual void processQueryResult( 
        IQueryResultProcessor* thisObject, 
        const std::string& queryResultToBeProcessed ) = 0;};

#endif // IQUERY_RESULT_PROCESSOR_H
