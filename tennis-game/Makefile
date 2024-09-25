# Variables
CXX = g++ -std=c++17
CFLAGS = -Wall -O3 -g -Wextra -Wno-unused-parameter
LDFLAGS = -lrt -lm -ljsoncpp -lcurl -lpthread

# Paths
RGB_LIB_DISTRIBUTION = ..
RGB_INCDIR = $(RGB_LIB_DISTRIBUTION)/include
RGB_LIBDIR = $(RGB_LIB_DISTRIBUTION)/lib
RGB_LIBRARY_NAME = rgbmatrix
RGB_LIBRARY = $(RGB_LIBDIR)/lib$(RGB_LIBRARY_NAME).a

# Google Test and Google Mock
GTEST_DIR = /home/adamsl/rpi-rgb-led-matrix/tennis-game/googletest
GTEST_INCDIR = $(GTEST_DIR)/googletest/include
GMOCK_INCDIR = $(GTEST_DIR)/googlemock/include
GTEST_LIBDIR = $(GTEST_DIR)/build/lib
GTEST_LIBS = -lgtest_main -lgtest -lgmock -lgmock_main -lpthread

# Include Directories
INCLUDES = -I$(GTEST_INCDIR) -I$(GMOCK_INCDIR) -I$(RGB_INCDIR) -I.

# Source Files
SRC_DIR = .
SRCS = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*/*.cpp)

# Exclude main.cpp
SRCS := $(filter-out $(SRC_DIR)/main.cpp, $(SRCS))

OBJS = $(SRCS:.cpp=.o)

# Binaries
BINARIES = tennis-game Mode1ScoreTest

# All Target
all: $(BINARIES)

# Tennis Game
tennis-game: $(OBJS)
	$(CXX) $(CFLAGS) $(INCLUDES) -o $@ $^ -L$(RGB_LIBDIR) -l$(RGB_LIBRARY_NAME) $(LDFLAGS)

# Mode1ScoreTest
Mode1ScoreTest: $(OBJS)
	$(CXX) $(CFLAGS) $(INCLUDES) -o $@ $^ -L$(GTEST_LIBDIR) $(GTEST_LIBS) -L$(RGB_LIBDIR) -l$(RGB_LIBRARY_NAME) $(LDFLAGS)

# Pattern Rule for Object Files
%.o: %.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c -o $@ $<

# Clean
clean:
	rm -f $(OBJS) $(BINARIES)

.PHONY: all clean
print-%:
	@echo $* = $($*)