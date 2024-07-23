CXX = g++
CXXFLAGS = -std=c++14 -I/usr/local/include
LDFLAGS = -L/usr/local/lib -lgtest -lgtest_main -pthread

TARGET = run_tests
SRCS = src/my_class.cpp tests/my_class_test.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS) $(LDFLAGS)

clean:
	rm -f $(TARGET)
