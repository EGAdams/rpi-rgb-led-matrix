
# Setting Up Google Unit Test in C++ Without CMake

## 1. Create The Project Structure

Organize your project with a structure like this:

```
my_project/
├── include/
│   └── my_class.h
├── src/
│   └── my_class.cpp
├── tests/
│   └── my_class_test.cpp
├── main.cpp
└── Makefile
```

## 2. Write The Code

### `include/my_class.h`:
```cpp
#ifndef MY_CLASS_H
#define MY_CLASS_H

class MyClass {
public:
    int add(int a, int b);
};

#endif // MY_CLASS_H
```

### `src/my_class.cpp`:
```cpp
#include "my_class.h"

int MyClass::add(int a, int b) {
    return a + b;
}
```

## 3. Write The Tests

### `tests/my_class_test.cpp`:
```cpp
#include <gtest/gtest.h>
#include "my_class.h"

TEST(MyClassTest, Add) {
    MyClass myClass;
    EXPECT_EQ(myClass.add(2, 3), 5);
    EXPECT_EQ(myClass.add(-1, 1), 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
```

## 4. Write The Makefile

### `Makefile`:
```makefile
CXX = g++
CXXFLAGS = -std=c++11 -I/usr/local/include
LDFLAGS = -L/usr/local/lib -lgtest -lgtest_main -pthread

TARGET = run_tests
SRCS = src/my_class.cpp tests/my_class_test.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
    $(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS) $(LDFLAGS)

clean:
    rm -f $(TARGET)
```

## 5. Build and Run The Tests

```sh
make
./run_tests
```

This setup avoids using CMake by leveraging a simple Makefile, which compiles your project and links it against Google Test. You can run your tests by executing the `run_tests` binary created by the Makefile.
