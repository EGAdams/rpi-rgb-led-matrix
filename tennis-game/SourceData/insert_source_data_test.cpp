#include "SourceData.h"
#include "../IQueryResultProcessor.h"
#include <iostream>
#include <string>
#include "/usr/include/jsoncpp/json/json.h"

// Mock implementation of IQueryResultProcessor for testing purposes
class MockQueryResultProcessor : public IQueryResultProcessor {
public:
    void processQueryResult( IQueryResultProcessor* thisObject, const std::string& queryResultToBeProcessed ) override {
        std::cout << "Processed Query Result: " << queryResultToBeProcessed << std::endl;
    }
};

void testInsertObject() {
    std::cout << "creating a mock query configuration..." << std::endl;
    ISourceQueryConfig queryConfig; // Create a mock query configuration

    std::cout << "setting the query configuration's object data..." << std::endl;
    queryConfig.object_data = "{\"name\":\"NewObject\",\"value\":\"12345\"}"; // Example data for insertion

    // Parse the JSON string to extract the name and set it as object_view_id
    Json::Value root;
    Json::Reader reader;
    if (reader.parse(queryConfig.object_data, root)) {
        queryConfig.object_view_id = root["name"].asString();
    } else {
        std::cerr << "Error: Failed to parse JSON data in query_config.object_data\n";
        return;
    }

    std::cout << "creating a source data configuration..." << std::endl;
    ISourceDataConfig sourceConfig; // Create a SourceData instance with a valid URL

    std::cout << "setting the source data configuration's url..." << std::endl;
    sourceConfig.url = "https://americansjewelry.com/libraries/local-php-api/index.php/object/insert/";

    std::cout << "creating a source data instance..." << std::endl;
    SourceData sourceData( sourceConfig );

    std::cout << "creating a mock query result processor..." << std::endl;
    MockQueryResultProcessor mockProcessor; // Create a MockQueryResultProcessor instance

    std::cout << "calling the insertObject method..." << std::endl;
    sourceData.insertObject( queryConfig, &mockProcessor ); // Test the insertObject method
}

int main() {
    std::cout << "SourceData::insertObject test started." << std::endl;
    testInsertObject();
    std::cout << "SourceData::insertObject test completed." << std::endl;
    return 0;
}

