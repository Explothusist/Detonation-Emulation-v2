# make
# make BUILD=debug
# make BUILD=release

BUILD ?= debug

SDL_CFLAGS = -ISDL-Drawing-Library\x86_64-w64-mingw32\include
SDL_LDFLAGS = -LSDL-Drawing-Library\x86_64-w64-mingw32\lib -lSDL3 -lSDL3_image -lSDL3_ttf
LDFLAGS = -lcomdlg32
DRAWING_LIB_OBJS = SDL-Drawing-Library\DrawingContext.o SDL-Drawing-Library\WindowHandler.o
INTERFACE_OBJS = menus\MenuHandler.o
EMULATOR_OBJS = emulator\cpu.o emulator\memory.o
HELPER_OBJS = $(INTERFACE_OBJS) $(EMULATOR_OBJS) filehandle.o utils.o

COMMON_FLAGS = -Wall -Wextra -std=c++17
DEBUG_FLAGS  = -O0 -g -Wswitch -Wswitch-enum -Wimplicit-fallthrough
RELEASE_FLAGS = -O2 -march=native

ifeq ($(BUILD),debug)
    CFLAGS = $(COMMON_FLAGS) $(DEBUG_FLAGS)
    TARGET = main.exe
else
    CFLAGS = $(COMMON_FLAGS) $(RELEASE_FLAGS)
    TARGET = DetonationEmulation.exe
endif

all: $(TARGET)

$(TARGET): main.cpp $(HELPER_OBJS) Drawing-Library
	g++ $(CFLAGS) -o $(TARGET) main.cpp $(HELPER_OBJS) $(DRAWING_LIB_OBJS) $(SDL_CFLAGS) $(SDL_LDFLAGS) $(LDFLAGS)

filehandle.o: filehandle.cpp
	g++ $(CFLAGS) -c filehandle.cpp -o filehandle.o

utils.o: utils.cpp
	g++ $(CFLAGS) -c utils.cpp -o utils.o $(SDL_CFLAGS)

menus\MenuHandler.o: menus\MenuHandler.cpp
	g++ $(CFLAGS) -c menus\MenuHandler.cpp -o menus\MenuHandler.o $(SDL_CFLAGS)

emulator\cpu.o: emulator\cpu.cpp
	g++ $(CFLAGS) -c emulator\cpu.cpp -o emulator\cpu.o

emulator\memory.o: emulator\memory.cpp
	g++ $(CFLAGS) -c emulator\memory.cpp -o emulator\memory.o

Drawing-Library:
	$(MAKE) -C SDL-Drawing-Library

clean:
	del $(TARGET)

cleanAll:
	del $(TARGET) $(HELPER_OBJS) $(DRAWING_LIB_OBJS)