SDL_CFLAGS = -ISDL-Drawing-Library/x86_64-w64-mingw32/include
SDL_LDFLAGS = -LSDL-Drawing-Library/x86_64-w64-mingw32/lib -lSDL3 -lSDL3_image -lSDL3_ttf
DRAWING_LIB_INCLUDE = SDL-Drawing-Library/DrawingContext.o SDL-Drawing-Library/WindowHandler.o
INTERFACE_INCLUDE = menus\MenuHandler.o
HELPER_INCLUDE = $(INTERFACE_INCLUDE)

main.exe: main.cpp $(HELPER_INCLUDE) Drawing-Library
	g++ -Wall -o main.exe main.cpp $(HELPER_INCLUDE) $(DRAWING_LIB_INCLUDE) $(SDL_CFLAGS) $(SDL_LDFLAGS)

menus\MenuHandler.o: menus\MenuHandler.cpp
	g++ -Wall -c menus\MenuHandler.cpp -o menus\MenuHandler.o

Drawing-Library:
	$(MAKE) -C SDL-Drawing-Library

clean:
	del main.exe

cleanAll:
	del main.exe $(HELPER_INCLUDE)