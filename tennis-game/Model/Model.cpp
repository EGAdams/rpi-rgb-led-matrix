#include "Model.h"

void Model::selectObject(const ISourceQueryConfig& queryConfig, IQueryResultProcessor* callbackObject) {
    sourceData->selectObject(queryConfig, callbackObject);
}

void Model::selectAllObjects(IQueryResultProcessor* callbackObject) {
    sourceData->selectAllObjects(callbackObject);
}

void Model::insertObject(const ISourceQueryConfig& queryConfig, IQueryResultProcessor* callbackObject) {
    sourceData->insertObject(queryConfig, callbackObject);
}

void Model::updateObject(const ISourceQueryConfig& queryConfig, IQueryResultProcessor* callbackObject) {
    sourceData->updateObject(queryConfig, callbackObject);
}
