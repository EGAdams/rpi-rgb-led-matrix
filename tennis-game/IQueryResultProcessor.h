#ifndef IQUERY_RESULT_PROCESSOR_H
#define IQUERY_RESULT_PROCESSOR_H

#include <string>

class IQueryResultProcessor {
public:
    virtual ~IQueryResultProcessor() {}
    virtual void processQueryResult(IQueryResultProcessor* thisObject, const std::string& queryResultToBeProcessed) = 0;
};
#endif // IQUERY_RESULT_PROCESSOR_H
