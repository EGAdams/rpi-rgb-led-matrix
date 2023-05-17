CFLAGS=-Wall -O3 -g -Wextra -Wno-unused-parameter
CXXFLAGS=$(CFLAGS)
OBJECTS=tennis-game/CanvasCreator.o tennis-game/FontLoader.o tennis-game/TextDrawer.o tennis-game/GameTimer.o tennis-game/Logger.o tennis-game/History.o tennis-game/GameState.o tennis-game/ScoreBoard.o tennis-game/Player.o panel_test.o draw_random_pix.o demo-main.o minimal-example.o c-example.o text-example.o tennis-game/tennis-game.o scrolling-text-example.o clock.o ledcat.o input-example.o pixel-mover.o
BINARIES=panel_test draw_random_pix demo minimal-example c-example text-example tennis-game/tennis-game scrolling-text-example clock ledcat input-example pixel-mover

# Where our library resides. It is assumed here that $(RGB_LIB_DISTRIBUTION) has a
# compiled library in lib/
RGB_LIB_DISTRIBUTION=..
RGB_INCDIR=$(RGB_LIB_DISTRIBUTION)/include
RGB_LIBDIR=$(RGB_LIB_DISTRIBUTION)/lib
RGB_LIBRARY_NAME=rgbmatrix
RGB_LIBRARY=$(RGB_LIBDIR)/lib$(RGB_LIBRARY_NAME).a
LDFLAGS+=-L$(RGB_LIBDIR) -l$(RGB_LIBRARY_NAME) -lrt -lm -lpthread

# To compile image-example
MAGICK_CXXFLAGS?=$(shell GraphicsMagick++-config --cppflags --cxxflags)
MAGICK_LDFLAGS?=$(shell GraphicsMagick++-config --ldflags --libs)

all : $(BINARIES)

$(RGB_LIBRARY): FORCE
	$(MAKE) -C $(RGB_LIBDIR)

# demo : demo-main.o $(RGB_LIBRARY)
draw_random_pix : draw_random_pix.o
minimal-example : minimal-example.o
input-example : input-example.o
text-example: text-example.o
tennis-game: tennis-game/tennis-game.o
scrolling-text-example : scrolling-text-example.o
clock : clock.o
ledcat : ledcat.o
pixel-mover : pixel-mover.o
panel_test : panel_test.o

# All the binaries that have the same name as the object file.
% : %.o $(RGB_LIBRARY)
	$(CXX) $< -o $@ $(LDFLAGS)

image-example.o : image-example.cc
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) $(MAGICK_CXXFLAGS) -c -o $@ $<

image-example: image-example.o $(RGB_LIBRARY)
	$(CXX) $< -o $@ $(LDFLAGS) $(MAGICK_LDFLAGS)

# Since the C example uses the C++ library underneath, which depends on C++
# runtime stuff, you still have to also link -lstdc++
c-example : c-example.o $(RGB_LIBRARY)
	$(CC) $< -o $@ $(LDFLAGS) -lstdc++

%.o : %.cc
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

%.o : %.c
	$(CC) -I$(RGB_INCDIR) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJECTS) $(BINARIES)

FORCE:
.PHONY: FORCE