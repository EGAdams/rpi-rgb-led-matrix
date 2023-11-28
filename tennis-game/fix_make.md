
# Persona
- World-class C++ developer
- Expert at debugging g++ compiler and linker errors

# Goal
- Debug the errors in the ouput of make.

# Header file for the class: 
```cpp
#ifndef MODEL_H
#define MODEL_H

#include "../SourceData/SourceData.h"
#include <functional>
#include <string>

class Model {
public:
    explicit Model(SourceData* sourceData) : sourceData(sourceData) {}
    
    void selectObject(const std::string& object_view_id, const std::function<void(const std::string&)>& callback);

    void selectAllObjects(const std::function<void()>& callback);
    
    void insertObject(const std::string& object_view_id, const std::string& object_data, const 
    std::function<void()>& callback);
    // In Model.h
  
    void updateObject(const std::string& object_view_id, const std::string& object_data, const std::function<void(const std::string&)>& callback);



private:
    SourceData* sourceData;
};
#endif // MODEL_H

```

# Cpp file for the class: 
```cpp
#include "Model.h"

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

```

# Makefile
```make
# google test
GTEST_DIR=/home/adamsl/rpi-rgb-led-matrix/tennis-game/googletest
GTEST_INCDIR=$(GTEST_DIR)/googletest/include
GTEST_LIBDIR=$(GTEST_DIR)/build/lib

#-lgtest_main -lgtest -lpthread <-- this is the order that works
GTEST_LIBS=-lgtest_main -lgtest -lpthread

CFLAGS=-Wall -O3 -g -Wextra -Wno-unused-parameter
CXXFLAGS=$(CFLAGS) -I$(GTEST_INCDIR)
# OBJECTS=SetHistoryTextTest.o SetHistoryText.o SetDrawer.o GameLedTranslator.o SubjectManager.o WebLiquidCrystal.o WatchTimer.o Inputs.o TieBreaker.o Mode1Score.o Mode1Functions.o ServeLeds.o Undo.o BatteryTest.o Reset.o SetLeds.o TieLeds.o Mode1WinSequences.o Mode2Functions.o MatchWinSequence.o TennisConstants.o GameLeds.o GameModes.o GameObject.o PinState.o PinInterface.o TranslateConstant.o PointLeds.o Arduino.o CanvasCreator.o FontLoader.o Drawer.o TextDrawer.o GameTimer.o Logger.o History.o GameState.o ScoreBoard.o Player.o tennis-game.o LoggerFactory.o LogObject.o LogObjectContainer.o LogObjectFactory.o MonitorLedClassObject.o MonitorLed.o JsonParser.o
BINARIES=tennis-game tennis-game-manual Mode1ScoreTest

# Where our library resides. It is assumed here that $(RGB_LIB_DISTRIBUTION) has a
# compiled library in lib/
RGB_LIB_DISTRIBUTION=..
RGB_INCDIR=$(RGB_LIB_DISTRIBUTION)/include
RGB_LIBDIR=$(RGB_LIB_DISTRIBUTION)/lib
RGB_LIBRARY_NAME=rgbmatrix
RGB_LIBRARY=$(RGB_LIBDIR)/lib$(RGB_LIBRARY_NAME).a
LDFLAGS+=-L$(RGB_LIBDIR) -l$(RGB_LIBRARY_NAME) -lrt -lm -lpthread -L$(GTEST_LIBDIR) $(GTEST_LIBS)

# To compile image-example
MAGICK_CXXFLAGS?=$(shell GraphicsMagick++-config --cppflags --cxxflags)
MAGICK_LDFLAGS?=$(shell GraphicsMagick++-config --ldflags --libs)

MAIN_OBJECTS=GameWinSequence.o SetWin.o SetHistoryText.o SetDrawer.o GameLedTranslator.o SubjectManager.o WebLiquidCrystal.o WatchTimer.o Inputs.o TieBreaker.o Mode1Score.o Mode1Functions.o ServeLeds.o Undo.o BatteryTest.o Reset.o SetLeds.o TieLeds.o Mode1WinSequences.o Mode2Functions.o MatchWinSequence.o TennisConstants.o GameLeds.o GameModes.o GameObject.o PinState.o PinInterface.o TranslateConstant.o PointLeds.o Arduino.o CanvasCreator.o FontLoader.o Drawer.o TextDrawer.o GameTimer.o Logger.o History.o GameState.o ScoreBoard.o Player.o tennis-game.o LogObject.o LogObjectContainer.o LogObjectFactory.o MonitorLedClassObject.o MonitorLed.o JsonParser.o LoggerFactory.o Model.o MonitoredObject.o

MAIN_OBJECTS_MANUAL=GameWinSequence.o SetWin.o SetHistoryText.o SetDrawer.o GameLedTranslator.o SubjectManager.o WebLiquidCrystal.o WatchTimer.o Inputs.o TieBreaker.o Mode1Score.o Mode1Functions.o ServeLeds.o Undo.o BatteryTest.o Reset.o SetLeds.o TieLeds.o Mode1WinSequences.o Mode2Functions.o MatchWinSequence.o TennisConstants.o GameLeds.o GameModes.o GameObject.o PinState.o PinInterface.o TranslateConstant.o PointLeds.o Arduino.o CanvasCreator.o FontLoader.o Drawer.o TextDrawer.o GameTimer.o Logger.o History.o GameState.o ScoreBoard.o Player.o tennis-game-manual.o, LogObject.o LogObjectContainer.o LogObjectFactory.o MonitorLedClassObject.o MonitorLed.o JsonParser.o LoggerFactory.o Model.o MonitoredObject.o

TEST_OBJECTS=GameState.o Player.o GameState.o GameTimer.o ScoreBoard.o SetDrawer.o Drawer.o CanvasCreator.o FontLoader.o GameTimer.o, LogObject.o LogObjectContainer.o LogObjectFactory.o
SCORE_TEST_OBJECTS=GameState.o Player.o GameState.o GameTimer.o ScoreBoard.o SetDrawer.o  Mode1Score.o Mode1ScoreTest.o, LogObject.o LogObjectContainer.o LogObjectFactory.o MonitorLedClassObject.o MonitorLed.o JsonParser.o LoggerFactory.o Model.o MonitoredObject.o

all : $(BINARIES)

$(RGB_LIBRARY): FORCE
	$(MAKE) -C $(RGB_LIBDIR)

tennis-game: $(MAIN_OBJECTS)
	$(CXX) $^ -o $@ $(LDFLAGS)

tennis-game-manual: GameWinSequence.o SetWin.o SetHistoryText.o SetDrawer.o GameLedTranslator.o SubjectManager.o WebLiquidCrystal.o WatchTimer.o Inputs.o TieBreaker.o Mode1Score.o Mode1Functions.o ServeLeds.o Undo.o BatteryTest.o Reset.o SetLeds.o TieLeds.o Mode1WinSequences.o Mode2Functions.o MatchWinSequence.o TennisConstants.o GameLeds.o GameModes.o GameObject.o PinState.o PinInterface.o TranslateConstant.o PointLeds.o Arduino.o CanvasCreator.o FontLoader.o Drawer.o TextDrawer.o GameTimer.o Logger.o History.o GameState.o ScoreBoard.o Player.o tennis-game-manual.o LogObject.o LogObjectContainer.o LogObjectFactory.o MonitorLedClassObject.o MonitorLed.o JsonParser.o LoggerFactory.o Model.o MonitoredObject.o
	$(CXX) $^ -o $@ $(LDFLAGS)


Model.o : Model/Model.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

MonitoredObject.o : MonitoredObject/MonitoredObject.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

LoggerFactory.o : LoggerFactory/LoggerFactory.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

JsonParser.o : JsonParser/JsonParser.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

MonitorLed.o : MonitorLed/MonitorLed.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

MonitorLedClassObject.o : MonitorLedClassObject/MonitorLedClassObject.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

LogObjectFactory.o : LogObjectFactory/LogObjectFactory.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

LogObjectContainer.o : LogObjectContainer/LogObjectContainer.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

LogObject.o : LogObject/LogObject.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

test-manual: GameWinSequence.o SetWin.o
	@echo "Temporary rule worked."

Mode1ScoreTest: $( SCORE_TEST_OBJECTS )
	$(CXX) $^ -o $@ $(LDFLAGS)

Mode1ScoreTest.o: Mode1ScoreTest/Mode1ScoreTest.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

GameWinSequence.o : GameWinSequence/GameWinSequence.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

SetWin.o : SetWin/SetWin.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

SetDrawer.o : SetDrawer/SetDrawer.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

GameLedTranslator.o : GameLedTranslator/GameLedTranslator.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

CanvasCreator.o : CanvasCreator/CanvasCreator.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

FontLoader.o : FontLoader/FontLoader.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

Drawer.o : Drawer/Drawer.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

TextDrawer.o : TextDrawer/TextDrawer.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

GameTimer.o : GameTimer/GameTimer.cpp
	$(CXX) -I$(RGB_INCDIR) -I../Arduino $(CXXFLAGS) -c -o $@ $<

GameState.o : GameState/GameState.cpp
	$(CXX) -I$(RGB_INCDIR) -I../Arduino $(CXXFLAGS) -c -o $@ $<

GameObject.o : GameObject/GameObject.cpp
	$(CXX) -I$(RGB_INCDIR) -I../Arduino $(CXXFLAGS) -c -o $@ $<

GameModes.o : GameModes/GameModes.cpp
	$(CXX) -I$(RGB_INCDIR) -I../Arduino $(CXXFLAGS) -c -o $@ $<

GameLeds.o : GameLeds/GameLeds.cpp
	$(CXX) -I$(RGB_INCDIR) -I../Arduino $(CXXFLAGS) -c -o $@ $<

Arduino.o : Arduino/Arduino.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

Logger.o : Logger/Logger.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

History.o : History/History.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

ScoreBoard.o : ScoreBoard/ScoreBoard.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

Player.o : Player/Player.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

PinInterface.o : PinInterface/PinInterface.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

TranslateConstant.o : TranslateConstant/TranslateConstant.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

PointLeds.o : PointLeds/PointLeds.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

PinState.o : PinState/PinState.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

TennisConstants.o : TennisConstants/TennisConstants.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

Mode1Functions.o : Mode1Functions/Mode1Functions.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

Mode2Functions.o : Mode2Functions/Mode2Functions.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

Mode1WinSequences.o : WinSequences/WinSequences.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

SetLeds.o : SetLeds/SetLeds.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

TieLeds.o : TieLeds/TieLeds.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

Reset.o : Reset/Reset.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

MatchWinSequence.o : MatchWinSequence/MatchWinSequence.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

BatteryTest.o : BatteryTest/BatteryTest.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

WebLiquidCrystal.o : WebLiquidCrystal/WebLiquidCrystal.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

Undo.o : Undo/Undo.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

ServeLeds.o : ServeLeds/ServeLeds.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

Mode1Score.o : Mode1Score/Mode1Score.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

TieBreaker.o : TieBreaker/TieBreaker.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

Inputs.o : Inputs/Inputs.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

WatchTimer.o : WatchTimer/WatchTimer.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

SubjectManager.o : SubjectManager/SubjectManager.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

SetHistoryText.o : SetHistoryText/SetHistoryText.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

tennis-game.o : tennis-game.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

tennis-game-manual.o : tennis-game-manual.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<



clean:
	rm -f $(MAIN_OBJECTS) $(TEST_OBJECTS) $(BINARIES)

FORCE:
.PHONY: FORCE

print-%:
	@echo $* = $($*)

```

# Make output
```
g++ -I../include -Wall -O3 -g -Wextra -Wno-unused-parameter -I/home/adamsl/rpi-rgb-led-matrix/tennis-game/googletest/googletest/include -c -o Model.o Model/Model.cpp
Model/Model.cpp: In member function ‘void Model::selectObject(const string&, const std::function<void(const std::__cxx11::basic_string<char>&)>&)’:
Model/Model.cpp:5:30: error: cannot convert ‘const string’ {aka ‘const std::__cxx11::basic_string<char>’} to ‘const ISourceQueryConfig&’
    5 |     sourceData->selectObject(object_view_id, callback);
      |                              ^~~~~~~~~~~~~~
      |                              |
      |                              const string {aka const std::__cxx11::basic_string<char>}
In file included from Model/Model.h:4,
                 from Model/Model.cpp:1:
Model/../SourceData/SourceData.h:14:49: note:   initializing argument 1 of ‘void SourceData::selectObject(const ISourceQueryConfig&, IQueryResultProcessor*)’
   14 |     void selectObject(const ISourceQueryConfig& queryConfig, IQueryResultProcessor* callbackObject);
      |                       ~~~~~~~~~~~~~~~~~~~~~~~~~~^~~~~~~~~~~
Model/Model.cpp: In member function ‘void Model::selectAllObjects(const std::function<void()>&)’:
Model/Model.cpp:9:34: error: cannot convert ‘const std::function<void()>’ to ‘IQueryResultProcessor*’
    9 |     sourceData->selectAllObjects(callback);
      |                                  ^~~~~~~~
      |                                  |
      |                                  const std::function<void()>
In file included from Model/Model.h:4,
                 from Model/Model.cpp:1:
Model/../SourceData/SourceData.h:13:50: note:   initializing argument 1 of ‘void SourceData::selectAllObjects(IQueryResultProcessor*)’
   13 |     void selectAllObjects(IQueryResultProcessor* callbackObject);
      |                           ~~~~~~~~~~~~~~~~~~~~~~~^~~~~~~~~~~~~~
Model/Model.cpp: In member function ‘void Model::insertObject(const string&, const string&, const std::function<void()>&)’:
Model/Model.cpp:14:67: error: no matching function for call to ‘SourceData::insertObject(const string&, const string&, const std::function<void()>&)’
   14 |     sourceData->insertObject(object_view_id, object_data, callback);
      |                                                                   ^
In file included from Model/Model.h:4,
                 from Model/Model.cpp:1:
Model/../SourceData/SourceData.h:15:10: note: candidate: ‘void SourceData::insertObject(const ISourceQueryConfig&, IQueryResultProcessor*)’
   15 |     void insertObject(const ISourceQueryConfig& queryConfig, IQueryResultProcessor* callbackObject);
      |          ^~~~~~~~~~~~
Model/../SourceData/SourceData.h:15:10: note:   candidate expects 2 arguments, 3 provided
Model/Model.cpp: In member function ‘void Model::updateObject(const string&, const string&, const std::function<void(const std::__cxx11::basic_string<char>&)>&)’:
Model/Model.cpp:18:67: error: no matching function for call to ‘SourceData::updateObject(const string&, const string&, const std::function<void(const std::__cxx11::basic_string<char>&)>&)’
   18 |     sourceData->updateObject(object_view_id, object_data, callback);
      |                                                                   ^
In file included from Model/Model.h:4,
                 from Model/Model.cpp:1:
Model/../SourceData/SourceData.h:16:10: note: candidate: ‘void SourceData::updateObject(const ISourceQueryConfig&, IQueryResultProcessor*)’
   16 |     void updateObject(const ISourceQueryConfig& queryConfig, IQueryResultProcessor* callbackObject);
      |          ^~~~~~~~~~~~
Model/../SourceData/SourceData.h:16:10: note:   candidate expects 2 arguments, 3 provided
make: *** [Makefile:47: Model.o] Error 1

```

# Header files for the class: 


    
### Header file: ../SourceData/SourceData.h    
```cpp
#ifndef SOURCE_DATA_H
#define SOURCE_DATA_H

#include "../ISourceDataConfig.h"
#include "../ISourceQueryConfig.h"
#include "../IQueryResultProcessor.h"

class SourceData {
public:
    SourceData(const ISourceDataConfig& configuration_object)
        : runnerObject(configuration_object.runner), url(configuration_object.url) {}
    
    void selectAllObjects(IQueryResultProcessor* callbackObject);
    void selectObject(const ISourceQueryConfig& queryConfig, IQueryResultProcessor* callbackObject);
    void insertObject(const ISourceQueryConfig& queryConfig, IQueryResultProcessor* callbackObject);
    void updateObject(const ISourceQueryConfig& queryConfig, IQueryResultProcessor* callbackObject);

private:
    FetchRunner* runnerObject;  // Replace RunnerType with the actual C++ type
    std::string url;
};

#endif // SOURCE_DATA_H
```

