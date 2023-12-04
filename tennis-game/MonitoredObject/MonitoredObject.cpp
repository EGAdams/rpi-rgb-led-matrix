#include "MonitoredObject.h"
#include <iostream>

MonitoredObject::MonitoredObject(const std::string& object_name ) {
    initializeModel( object_name );
}

void MonitoredObject::initializeModel(const std::string& object_name) {
    object_view_id = object_name.find("_") != std::string::npos ? object_name : object_name + "_CPP";
    ISourceDataConfig config; // Create or obtain a suitable ISourceDataConfig object
    //... other initializations for config as needed
    model = std::make_unique<Model>(new SourceData(config));
    monitorLed = MonitorLed();
    updateDataConfig();
    // Rest of the code...
}


void MonitoredObject::logUpdate(const std::string& message) {
    ISourceQueryConfig queryConfig = ISourceQueryConfig();
    IQueryResultProcessor* processor = this;
    if ( object_view_id.empty()) {
        std::cout << "*** ERROR: object needs an id to log. ***" << std::endl;
        return;
    }
    if (message.find("ERROR") != std::string::npos) {
        monitorLed.setFail();
    }
    if (message.find("finished") != std::string::npos) {
        monitorLed.setPass();
    }
    monitorLed.setLedText(message);
    logObjects.push_back(logObjectFactory.createLogObject(message, object_view_id));
    updateDataConfig();
    model->updateObject( queryConfig, processor );
}

std::string MonitoredObject::getObjectViewId() const {
    return object_view_id;
}

void MonitoredObject::processQueryResult( IQueryResultProcessor* iqrp, const std::string& results ) {
    Json::Value root;
    Json::CharReaderBuilder builder;
    std::unique_ptr<Json::CharReader> reader(builder.newCharReader());

    std::string errors;
    bool parsingSuccessful = reader->parse(results.c_str(), results.c_str() + results.size(), &root, &errors);
    if (!parsingSuccessful) {
        // Handle errors
        std::cerr << "Failed to parse JSON: " << errors << std::endl;
        return;
    }

    // Use the 'root' Json::Value as needed
    std::cout << root.toStyledString() << std::endl;
}


void MonitoredObject::updateDataConfig() {
    // Create JSON representation of this object
    Json::Value objJson;
    objJson["object_view_id"] = object_view_id;
    // Populate other fields as needed
    Json::StreamWriterBuilder builder;
    data_config = Json::writeString(builder, objJson);
}
