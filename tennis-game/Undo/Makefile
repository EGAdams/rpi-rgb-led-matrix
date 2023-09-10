# Variable definitions
GTEST_DIR := /home/adamsl/linuxBash/SMOL_AI/tennis_unit_tests/googletest/build/lib
GMOCK_DIR := /home/adamsl/linuxBash/SMOL_AI/tennis_unit_tests/googletest/build/lib

CXX = g++
CXXFLAGS = -Wall -Wextra -g -I$(GTEST_DIR)/include -I../../include
LDFLAGS = -L$(GTEST_DIR)/lib -L../../lib -lgtest -lgtest_main -lgmock -lgmock_main -lrgbmatrix -pthread
TARGET = run_tests

# Define our source and object files
SOURCES =   ../Mode1Score/Mode1Score.cpp \
            ../Mode1TieBreaker/Mode1TieBreaker.cpp \
            ../GameLeds/GameLeds.cpp \
            ../GameTimer/GameTimer.cpp \
            ../GameState/GameState.cpp \
            ../GameWinSequence/GameWinSequence.cpp \
            ../GameLedTranslator/GameLedTranslator.cpp \
            ../MatchWinSequence/MatchWinSequence.cpp \
            ../Inputs/Inputs.cpp \
            ../Logger/Logger.cpp \
            ../PointLeds/PointLeds.cpp \
            ../PinState/PinState.cpp \
            ../PinInterface/PinInterface.cpp \
            ../Player/Player.cpp \
            ../Reset/Reset.cpp \
            ../ScoreBoard/ScoreBoard.cpp \
            ../SetHistoryText/SetHistoryText.cpp \
            ../SetLeds/SetLeds.cpp \
            ../ServeLeds/ServeLeds.cpp \
            ../SetWin/SetWin.cpp \
            ../SetDrawer/SetDrawer.cpp \
            ../Undo/Undo.cpp \
            ../TennisConstants/TennisConstants.cpp \
            ../TieLeds/TieLeds.cpp \
            ../History/History.cpp \
            ../WatchTimer/WatchTimer.cpp \
            ../FontLoader/FontLoader.cpp \
            ../WinSequences/WinSequences.cpp \
            ../TranslateConstant/TranslateConstant.cpp \
            UndoTest.cpp \

OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

test: $(TARGET)
	./$(TARGET)