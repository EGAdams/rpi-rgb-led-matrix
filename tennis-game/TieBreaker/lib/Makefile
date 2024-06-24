# Makefile for the lib directory

# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -O2

# Target library
TARGET = librgbmatrix.a

# Source files
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

# Default target
all: $(TARGET)

# Create the static library
$(TARGET): $(OBJECTS)
	ar rcs $@ $^

# Clean up
clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean
