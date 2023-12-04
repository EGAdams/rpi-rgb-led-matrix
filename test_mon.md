# Your role
- Expert at C++ project build debugging

# Your task
- Write Python code to debug the given make error
```bash
adamsl@DESKTOP-76N0UOT:~/linuxBash/SMOL_AI/tennis_unit_tests/Mode1Score$ make
g++ -Wall -Wextra -g -I/home/adamsl/linuxBash/SMOL_AI/tennis_unit_tests/googletest/build/lib/include -I../../include -c Mode1ScoreTest.cpp -o Mode1ScoreTest.o
Mode1ScoreTest.cpp:1:10: fatal error: gtest/gtest.h: No such file or directory
 #include <gtest/gtest.h>
          ^~~~~~~~~~~~~~~
compilation terminated.
Makefile:52: recipe for target 'Mode1ScoreTest.o' failed
make: *** [Mode1ScoreTest.o] Error 1
adamsl@DESKTOP-76N0UOT:~/linuxBash/SMOL_AI/tennis_unit_tests/Mode1Score$
```