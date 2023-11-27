#ifndef MODEL_H
#define MODEL_H

#include "../SourceData/SourceData.h"
#include <functional>
#include <string>

class Model {
public:
    explicit Model(SourceData* sourceData);
    
    void selectObject(const std::string& object_view_id, const std::function<void(const std::string&)>& callback);
    void selectAllObjects(const std::function<void()>& callback);
    void insertObject(const std::string& object_view_id, const std::string& object_data, const std::function<void()>& callback);
    // In Model.h
  void updateObject(const std::string& object_view_id, const std::string& object_data, const std::function<void(const std::string&)>& callback);



private:
    SourceData* sourceData;
};
#endif // MODEL_H
