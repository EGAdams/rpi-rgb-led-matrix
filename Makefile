# This toplevel Makefile compiles the library in the lib subdirectory.
# If you want to see how to integrate the library in your own projects, check
# out the sub-directories examples-api-use/ and utils/
RGB_LIBDIR=./lib
RGB_LIBRARY_NAME=rgbmatrix
RGB_LIBRARY=$(RGB_LIBDIR)/lib$(RGB_LIBRARY_NAME).a

# Some language bindings.
PYTHON_LIB_DIR=bindings/python
CSHARP_LIB_DIR=bindings/c\#

all : $(RGB_LIBRARY)

$(RGB_LIBRARY): FORCE
	$(MAKE) -C $(RGB_LIBDIR)
	$(MAKE) -C examples-api-use

clean:
	$(MAKE) -C lib clean
	$(MAKE) -C utils clean
	$(MAKE) -C examples-api-use clean
	$(MAKE) -C $(PYTHON_LIB_DIR) clean

build-csharp:
	$(MAKE) -C $(CSHARP_LIB_DIR) build

build-python: $(RGB_LIBRARY)
	$(MAKE) -C $(PYTHON_LIB_DIR) build

install-python: build-python
	$(MAKE) -C $(PYTHON_LIB_DIR) install

TieBreakerTest.o: TieBreakerTest.c
	$(CC) $(CFLAGS) -c TieBreakerTest.c -o TieBreakerTest.o

FORCE:
.PHONY: FORCE
../Mode1WinSequences/Mode1WinSequences.o: ../Mode1WinSequences/Mode1WinSequences.cpp
	$(CXX) $(CXXFLAGS) -c ../Mode1WinSequences/Mode1WinSequences.cpp -o ../Mode1WinSequences/Mode1WinSequences.o

TieBreakerTest.o: TieBreakerTest.c
	$(CC) $(CFLAGS) -c TieBreakerTest.c -o TieBreakerTest.o
TieBreakerTest: TieBreakerTest.o ../Mode1WinSequences/Mode1WinSequences.o
	g++ -std=c++14 -I../ -I../../lib -I../../lib/gtest/include -o TieBreakerTest TieBreakerTest.o ../Player/Player.o ../GameState/GameState.o ../PinInterface/PinInterface.o ../PinState/PinState.o ../TieBreaker/TieBreaker.o ../TranslateConstant/TranslateConstant.o ../PointLeds/PointLeds.o ../GameLeds/GameLeds.o ../ServeLeds/ServeLeds.o ../SetLeds/SetLeds.o ../Undo/Undo.o ../GameTimer/GameTimer.o ../SetWin/SetWin.o ../Inputs/Inputs.o ../WatchTimer/WatchTimer.o ../ScoreBoard/ScoreBoard.o ../Mode1WinSequences/Mode1WinSequences.o -L../../lib/gtest -lgtest -lgtest_main -pthread
Mode1ScoreTest: TieBreakerTest.o
	$(CC) $(CFLAGS) Mode1ScoreTest.c TieBreakerTest.o -o Mode1ScoreTest

TieBreakerTest.o: TieBreakerTest.c
	$(CC) $(CFLAGS) -c TieBreakerTest.c -o TieBreakerTest.o
