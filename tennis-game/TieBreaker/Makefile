# Makefile for TieBreaker tests

CXX = g++
CXXFLAGS = -std=c++14 -I../ -I../../lib -I../../lib/gtest/include
LDFLAGS = -L../../lib/gtest -lgtest -lgtest_main -pthread

# Source files
SRCS = ../Mode1Score/TieBreakerTest.cpp ../Player/Player.cpp ../GameState/GameState.cpp ../PinInterface/PinInterface.cpp ../PinState/PinState.cpp
OBJS = $(SRCS:.cpp=.o)

# Target executable
TARGET = TieBreakerTest

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
