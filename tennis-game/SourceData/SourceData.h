#ifndef SOURCEDATA_H
#define SOURCEDATA_H

#include <string>
#include <functional>

class SourceData {
public:
    explicit SourceData(const std::string& url);
    
    void selectAllObjects(const std::function<void()>& callback);
    void selectObject(const std::string& object_view_id, const std::function<void(const std::string&)>& callback);
    void insertObject(const std::string& object_view_id, const std::string& object_data, const std::function<void()>& callback);
    void updateObject(const std::string& object_view_id, const std::string& object_data, const std::function<void()>& callback);

private:
    std::string url;
    std::string api_path;

    // Helper methods for making HTTP requests
    void get(const APIConfigStruct& config, std::function<void(const std::string&)> callback);
    void post(const std::string& api_path, const std::string& data, const std::function<void()>& callback);
};

#endif // SOURCEDATA_H
