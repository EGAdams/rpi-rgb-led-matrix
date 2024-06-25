# google test
GTEST_DIR=/home/adamsl/rpi-rgb-led-matrix/tennis-game/googletest
GTEST_INCDIR=$(GTEST_DIR)/googletest/include
GTEST_LIBDIR=$(GTEST_DIR)/build/lib

#-lgtest_main -lgtest -lpthread <-- this is the order that works
GTEST_LIBS=-lgtest_main -lgtest -lpthread

CXX=g++
CFLAGS=-Wall -O3 -g -Wextra -Wno-unused-parameter
CXXFLAGS=$(CFLAGS) -I$(GTEST_INCDIR) -I$(RGB_INCDIR)

RGB_LIB_DISTRIBUTION=../../
RGB_INCDIR=$(RGB_LIB_DISTRIBUTION)/include
RGB_LIBDIR=$(RGB_LIB_DISTRIBUTION)/lib
RGB_LIBRARY_NAME=rgbmatrix
RGB_LIBRARY=$(RGB_LIBDIR)/lib$(RGB_LIBRARY_NAME).a

LDFLAGS+=-L$(RGB_LIBDIR) -l$(RGB_LIBRARY_NAME) -lrt -lm -L$(GTEST_LIBDIR) $(GTEST_LIBS) -ljsoncpp -lcurl -lpthread

TEST_OBJECTS=../TieLeds/TieLeds.o ../GameLedTranslator/GameLedTranslator.o ../GameState/GameState.o ../Player/Player.o ../GameTimer/GameTimer.o ../ScoreBoard/ScoreBoard.o ../SetDrawer/SetDrawer.o ../SetHistoryText/SetHistoryText.o ../Drawer/Drawer.o ../CanvasCreator/CanvasCreator.o ../FontLoader/FontLoader.o ../LogObject/LogObject.o ../LogObjectContainer/LogObjectContainer.o ../LogObjectFactory/LogObjectFactory.o ../MonitorLedClassObject/MonitorLedClassObject.o ../MonitorLed/MonitorLed.o ../JsonParser/JsonParser.o ../LoggerFactory/LoggerFactory.o ../Model/Model.o ../MonitoredObject/MonitoredObject.o ../SourceData/SourceData.o ../FetchRunner/FetchRunner.o ../PinInterface/PinInterface.o ../PinState/PinState.o ../History/History.o ../Mode1Score/Mode1Score.o TieBreakerTest.o ../TranslateConstant/TranslateConstant.o ../Logger/Logger.o TieBreaker.o ../PointLeds/PointLeds.o ../GameLeds/GameLeds.o ../SetLeds/SetLeds.o ../WinSequences/WinSequences.o ../Undo/Undo.o ../ServeLeds/ServeLeds.o ../GameWinSequence/GameWinSequence.o ../SetWin/SetWin.o ../MatchWinSequence/MatchWinSequence.o ../Inputs/Inputs.o ../WatchTimer/WatchTimer.o ../Reset/Reset.o

all: TieBreakerTest

TieBreakerTest: $(TEST_OBJECTS)
	$(CXX) $(CXXFLAGS) -o TieBreakerTest $(TEST_OBJECTS) $(LDFLAGS)

TieBreakerTest.o: TieBreakerTest.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

TieBreaker.o: TieBreaker.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(TEST_OBJECTS) TieBreakerTest

FORCE:
.PHONY: FORCE

print-%:
	@echo $* = $($*)
