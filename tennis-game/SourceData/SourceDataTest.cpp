#include "SourceData.h"
#include "../IQueryResultProcessor.h"
#include <iostream>
#include <string>

// Mock implementation of IQueryResultProcessor for testing purposes
class MockQueryResultProcessor : public IQueryResultProcessor {
public:
    void processQueryResult( IQueryResultProcessor* thisObject, const std::string& queryResultToBeProcessed ) override {
        std::cout << "Processed Query Result: " << queryResultToBeProcessed << std::endl;
    }
};

void testSelectObject() {
    std::cout << "creating a mock query configuration..." << std::endl;
    ISourceQueryConfig queryConfig; // Create a mock query configuration

    std::cout << "setting the query configuration's object_view_id..." << std::endl;
    queryConfig.object_view_id = "McbaStartupMonitor_2021"; // Assuming this is a valid ID for the test

    std::cout << "creating a source data configuration..." << std::endl;
    ISourceDataConfig sourceConfig; // Create a SourceData instance with a valid URL

    std::cout << "setting the source data configuration's url..." << std::endl;
    sourceConfig.url = "https://americansjewelry.com/libraries/local-php-api/index.php/object/select/";

    std::cout << "creating a source data instance..." << std::endl;
    SourceData sourceData( sourceConfig );

    std::cout << "creating a mock query result processor..." << std::endl;
    MockQueryResultProcessor mockProcessor; // Create a MockQueryResultProcessor instance

    std::cout << "calling the selectObject method..." << std::endl;
    sourceData.selectObject( queryConfig, &mockProcessor ); // Test the selectObject method
}

int main() {
    std::cout << "SourceData::selectObject test started." << std::endl;
    testSelectObject();
    std::cout << "SourceData::selectObject test completed." << std::endl;
    return 0; }
