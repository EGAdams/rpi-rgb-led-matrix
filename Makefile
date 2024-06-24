# This toplevel Makefile compiles the library in the lib subdirectory.
# If you want to see how to integrate the library in your own projects, check
# out the sub-directories examples-api-use/ and utils/
RGB_LIBDIR=./lib
RGB_LIBRARY_NAME=rgbmatrix
RGB_LIBRARY=$(RGB_LIBDIR)/lib$(RGB_LIBRARY_NAME).a

# Some language bindings.
PYTHON_LIB_DIR=bindings/python
CSHARP_LIB_DIR=bindings/c\#

all: $(RGB_LIBRARY)

$(RGB_LIBRARY): | $(RGB_LIBDIR)
	$(MAKE) -C $(RGB_LIBDIR)

$(RGB_LIBDIR):
	mkdir -p $(RGB_LIBDIR)

clean:
	$(MAKE) -C lib clean
	$(MAKE) -C utils clean
	$(MAKE) -C $(PYTHON_LIB_DIR) clean

build-csharp:
	$(MAKE) -C $(CSHARP_LIB_DIR) build

build-python: $(RGB_LIBRARY)
	$(MAKE) -C $(PYTHON_LIB_DIR) build

install-python: build-python
	$(MAKE) -C $(PYTHON_LIB_DIR) install

FORCE:
.PHONY: FORCE

../Mode1WinSequences/Mode1WinSequences.o: ../Mode1WinSequences/Mode1WinSequences.cpp
	@if [ -f ../Mode1WinSequences/Mode1WinSequences.cpp ]; then \
		$(CXX) $(CXXFLAGS) -c ../Mode1WinSequences/Mode1WinSequences.cpp -o ../Mode1WinSequences/Mode1WinSequences.o; \
	else \
		echo "Error: ../Mode1WinSequences/Mode1WinSequences.cpp not found."; \
		exit 1; \
	fi

TieBreakerTest.o: TieBreakerTest.cpp
	$(CXX) $(CXXFLAGS) -c TieBreakerTest.cpp -o TieBreakerTest.o

TieBreakerTest: TieBreakerTest.o ../Mode1WinSequences/Mode1WinSequences.o
	g++ -std=c++14 -I../ -I../../lib -I../../lib/gtest/include -o TieBreakerTest TieBreakerTest.o ../Player/Player.o ../GameState/GameState.o ../PinInterface/PinInterface.o ../PinState/PinState.o ../TieBreaker/TieBreaker.o ../TranslateConstant/TranslateConstant.o ../PointLeds/PointLeds.o ../GameLeds/GameLeds.o ../ServeLeds/ServeLeds.o ../SetLeds/SetLeds.o ../Undo/Undo.o ../GameTimer/GameTimer.o ../SetWin/SetWin.o ../Inputs/Inputs.o ../WatchTimer/WatchTimer.o ../ScoreBoard/ScoreBoard.o ../Mode1WinSequences/Mode1WinSequences.o -L../../lib/gtest -lgtest -lgtest_main -pthread

Mode1ScoreTest: TieBreakerTest.o
	$(CXX) $(CXXFLAGS) Mode1ScoreTest.cpp TieBreakerTest.o -o Mode1ScoreTest
