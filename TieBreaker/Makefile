# Makefile for running TieBreaker tests

CXX = g++
CXXFLAGS = -std=c++11 -I../ -I../../gtest/include
LDFLAGS = -L../../gtest/lib -lgtest -lgtest_main -pthread

# Source files
SRCS = ../Mode1Score/TieBreakerTest.cpp TieBreaker.cpp ../Player/Player.cpp ../GameState/GameState.cpp ../PinInterface/PinInterface.cpp ../PinState/PinState.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Executable
TARGET = TieBreakerTest

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
