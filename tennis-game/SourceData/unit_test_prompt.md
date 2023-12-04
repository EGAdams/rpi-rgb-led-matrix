
# Persona
- World-class C++ developer

# Goal
- Create simple tests for the SourceData class.  Don't make unit tests, make simple tests that can be run manually.

# Header file for the class: SourceData
```cpp
#ifndef SOURCEDATA_H
#define SOURCEDATA_H

#include <string>
#include <functional>
#include "../FetchRunner/FetchRunner.h"
#include "../MonitoredObjectStructures.h"

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

```

# Cpp file for the class: SourceData
```cpp
#include "SourceData.h"
#include <iostream> // For demonstration purposes

SourceData::SourceData(const std::string& url) : url(url) {}

// Function to convert APIConfig to JSON string
std::string toJson(const APIConfigStruct& config) {
    // Implement JSON serialization here
}

void SourceData::selectAllObjects(const std::function<void()>& callback) {
    api_path = url + "object/selectAll";
    APIConfigStruct config;
    config.api_path = api_path;
    get(config, [callback](const std::string&) {
        callback();
    });
}

void SourceData::selectObject(const std::string& object_view_id, const std::function<void(const std::string&)>& callback) {
    api_path = url + "object/select";
    APIConfigStruct config;
    config.api_path = api_path;
    get(config, [callback](const std::string& response) {
        callback(response);
    }); // Pass object_view_id in a real implementation
}

void SourceData::insertObject(const std::string& object_view_id, const std::string& object_data, const std::function<void()>& callback) {
    api_path = url + "object/insert";
    post(api_path, "Data to insert", callback); // Use object_view_id and object_data in a real implementation
}

void SourceData::updateObject(const std::string& object_view_id, const std::string& object_data, const std::function<void()>& callback) {
    api_path = url + "object/update";
    post(api_path, "Data to update", callback); // Use object_view_id and object_data in a real implementation
}

void SourceData::get(const APIConfigStruct& config, std::function<void(const std::string&)> callback) {
    std::string apiPath = url + config.api_path;
    std::string jsonConfig = toJson(config);

    FetchRunner runner(apiPath);
    runner.run("GET", jsonConfig, [callback](const std::string& response) {
        callback(response);
    });
}

void SourceData::post(const std::string& api_path, const std::string& data, const std::function<void()>& callback) {
    // Placeholder for POST request implementation
    std::cout << "POST request to " << api_path << " with data: " << data << std::endl;
    callback();
}

```

# Header files for the class: SourceData


    
### Header file: ../FetchRunner/FetchRunner.h    
```cpp
#ifndef FETCHRUNNER_H
#define FETCHRUNNER_H

#include <string>
#include <functional>
#include <curl/curl.h>

class FetchRunner {
public:
    FetchRunner(const std::string& config);
    ~FetchRunner();

    void run(const std::string& apiArgsType, const std::string& apiArgs, std::function<void(const std::string&)> callback);

private:
    std::string url;
    struct curl_slist* url_encoded_header;
    struct curl_slist* json_header;
    CURL* curl;

    void initializeCurl();
    void setupHeaders();
    static size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp);
};

#endif // FETCHRUNNER_H


```


    
### Header file: ../MonitoredObjectStructures.h    
```cpp
#include <string>

struct MonitorLedConfigStruct {
    std::string new_id;
    std::string table;
};

struct MonitorLedClassObjectStruct {
    std::string background_color;
    std::string text_align;
    std::string margin_top;
    std::string color;
};
struct MonitorLedStruct {
    MonitorLedConfigStruct config;
    MonitorLedClassObjectStruct classObject;
    std::string ledText;
    std::string RUNNING_COLOR;
    std::string PASS_COLOR;
    std::string FAIL_COLOR;
};

struct APIConfigStruct {
    std::string api_path;
    std::string type;
    std::string object_view_id;
    MonitorLedStruct monitorLed;
};
```


## Working example Google Unit Test for the the Mode1Score class

```cpp
#include "Mode1Score.h"
class Mode1ScoreTest : public ::testing::Test {
protected:
    Player* _player1;
    Player* _player2;
    PinInterface* _pinInterface;
    GameState* _gameState;
    History* _history;
    Mode1Score* _mode1Score;

    void SetUp() override;
    void TearDown() override;
};

void Mode1ScoreTest::SetUp() {
    _gameState = new GameState();
    _player1 = new Player( _gameState, PLAYER_1_INITIALIZED );
    _player2 = new Player( _gameState, PLAYER_2_INITIALIZED );
    _player1->setOpponent( _player2 ); _player2->setOpponent( _player1 );
    std::map<std::string, int> pin_map;
    PinState* pin_state = new PinState( pin_map );
    _pinInterface = new PinInterface( pin_state );
    _history = new History();
    _mode1Score = new Mode1Score( _player1, _player2, _pinInterface, _gameState, _history );
}

// Tear down the test fixture
void Mode1ScoreTest::TearDown() {
    delete _mode1Score;
    delete _history;
    delete _gameState;
    delete _pinInterface;
    delete _player2;
    delete _player1;
}


///////////////// Test case: Test Mode 1 P1 Score Less Than 3 Points //////////
TEST_F( Mode1ScoreTest, TestMode1P1Score_LessThan3Points ) {
    _player1->setPoints( 2 );
    _player2->setPoints( 1 );
    _mode1Score->updateScore( _player1 );
    EXPECT_EQ( _player1->getPoints(), 2);
    EXPECT_EQ( _player2->getPoints(), 1 );
    // Check other changes made by the method
}
////////////////////////////////////////////////////////////////////////////////
```
