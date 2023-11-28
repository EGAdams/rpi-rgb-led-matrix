#include "Model.h"

Model::Model(SourceData* sourceData) : sourceData(sourceData) {}

void Model::selectObject(const std::string& object_view_id,
                         const std::function<void(const std::string&)>& callback) {
    sourceData->selectObject(object_view_id, callback);
}

void Model::selectAllObjects(const std::function<void()>& callback) {
    sourceData->selectAllObjects(callback);
}

void Model::insertObject(const std::string& object_view_id,
                         const std::string& object_data, const std::function<void()>& callback) {
    sourceData->insertObject(object_view_id, object_data, callback);
}

void Model::updateObject(const std::string& object_view_id, const std::string& object_data, const std::function<void(const std::string&)>& callback) {
    sourceData->updateObject(object_view_id, object_data, callback);
}
