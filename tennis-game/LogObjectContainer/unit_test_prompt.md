
# Persona
- World-class C++ developer
- Expert at using C++ Google Unit Test Framework

# Goal
- Create a detailed exhaustive Google Unit test for the class: LogObjectContainer

# Header file for the class: LogObjectContainer
```cpp
#ifndef LOGOBJECTCONTAINER_H
#define LOGOBJECTCONTAINER_H

#include "../LogObject/LogObject.h"
#include <vector>
#include <memory>

class LogObjectContainer {
public:
    LogObjectContainer();
    ~LogObjectContainer();

    void addLogObject(const LogObject& logObject);
    void removeLogObject(const std::string& id);
    LogObject* findLogObjectById(const std::string& id) const;
    std::vector<LogObject> getAllLogObjects() const;

private:
    std::vector<std::shared_ptr<LogObject>> logObjects;
};

#endif // LOGOBJECTCONTAINER_H

```

# Cpp file for the class: LogObjectContainer
```cpp
#include "LogObjectContainer.h"
#include <algorithm>

LogObjectContainer::LogObjectContainer() {}

LogObjectContainer::~LogObjectContainer() {}

void LogObjectContainer::addLogObject(const LogObject& logObject) {
    logObjects.push_back(std::make_shared<LogObject>(logObject));
}

void LogObjectContainer::removeLogObject(const std::string& id) {
    logObjects.erase(std::remove_if(logObjects.begin(), logObjects.end(),
                                    [&id](const std::shared_ptr<LogObject>& obj) { return obj->getId() == id; }),
                     logObjects.end());
}

LogObject* LogObjectContainer::findLogObjectById(const std::string& id) const {
    auto it = std::find_if(logObjects.begin(), logObjects.end(),
                           [&id](const std::shared_ptr<LogObject>& obj) { return obj->getId() == id; });
    if (it != logObjects.end()) {
        return it->get();
    }
    return nullptr;
}

std::vector<LogObject> LogObjectContainer::getAllLogObjects() const {
    std::vector<LogObject> allObjects;
    for (const auto& obj : logObjects) {
        allObjects.push_back(*obj);
    }
    return allObjects;
}

```

# Header files for the class: LogObjectContainer


    
### Header file: ../LogObject/LogObject.h    
```cpp
#ifndef LOGOBJECT_H
#define LOGOBJECT_H

#include <string>
#include <cstdint>

class LogObject {
public:
    LogObject();
    ~LogObject();

    const std::string& getId() const;
    void setId(const std::string& id);

    uint64_t getTimestamp() const;
    void setTimestamp(uint64_t timestamp);

    const std::string& getMessage() const;
    void setMessage(const std::string& message);

    const std::string& getMethod() const;
    void setMethod(const std::string& method);

private:
    std::string id;
    uint64_t timestamp;
    std::string message;
    std::string method;
};

#endif // LOGOBJECT_H

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
