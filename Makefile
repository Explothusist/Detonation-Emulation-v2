# Makefile
# Usage:
#   make               (default debug build)
#   make BUILD=release
#   make CXX=g++       (or clang++)
#   make CXX=cl        (MSVC)
#   make clean
#   make cleanAll

BUILD ?= debug
CXX   ?= g++
BUILD_DIR ?= build

# --- Platform-specific commands ---
ifeq ($(OS),Windows_NT)
	RM = del /Q
	RMDIR = rmdir /S /Q
else
	RM = rm -f
	RMDIR = rm -rf
endif

# --- SDL paths ---
SDL_BASE = SDL3-3.4.0
SDL_IMAGE = SDL3_image-3.2.6
SDL_TTF = SDL3_ttf-3.2.2

SDL_CFLAGS = -ISDL-Drawing-Library/$(SDL_BASE)/include -ISDL-Drawing-Library/$(SDL_IMAGE)/include -ISDL-Drawing-Library/$(SDL_TTF)/include
SDL_LDFLAGS = -LSDL-Drawing-Library/$(SDL_BASE)/lib -LSDL-Drawing-Library/$(SDL_IMAGE)/lib -LSDL-Drawing-Library/$(SDL_TTF)/lib -lSDL3 -lSDL3_image -lSDL3_ttf
SDL_LDLIBS  = -lSDL3 -lSDL3_image -lSDL3_ttf

# --- Source files ---
DRAWING_LIB_SRCS = SDL-Drawing-Library/DrawingContext.cpp SDL-Drawing-Library/WindowHandler.cpp
INTERFACE_SRCS = menus/MenuHandler.cpp
EMULATOR_SRCS  = emulator/apu.cpp emulator/cpu.cpp emulator/emulator_frontend.cpp emulator/memory.cpp emulator/opcodes.cpp emulator/ppu.cpp emulator/components/controller.cpp emulator/components/serial.cpp emulator/components/timer.cpp emulator/components/interrupts.cpp emulator/components/emulator_log.cpp
HELPER_SRCS    = $(INTERFACE_SRCS) $(EMULATOR_SRCS) filehandle.cpp utils.cpp

# --- Object files in build/ ---
DRAWING_LIB_OBJS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(DRAWING_LIB_SRCS))
HELPER_OBJS      = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(HELPER_SRCS))

# --- Flags ---
CPPFLAGS += $(SDL_CFLAGS)

ifeq ($(CXX),cl)
	COMMON_CXXFLAGS  = /std:c++17 /W4
	DEBUG_CXXFLAGS   = /Od /Zi
	RELEASE_CXXFLAGS = /O2
	LDFLAGS         += /link
else
	COMMON_CXXFLAGS  = -Wall -Wextra -std=c++17
	DEBUG_CXXFLAGS   = -O0 -g -Wswitch -Wswitch-enum -Wimplicit-fallthrough
	RELEASE_CXXFLAGS = -O2 -march=native

	# Auto dependency generation (GCC/Clang)
	CXXFLAGS += -MMD -MP
endif

ifeq ($(BUILD),debug)
	CXXFLAGS += $(COMMON_CXXFLAGS) $(DEBUG_CXXFLAGS)
	TARGET    = main.exe
else
	CXXFLAGS += $(COMMON_CXXFLAGS) $(RELEASE_CXXFLAGS)
	TARGET    = DetonationEmulation.exe
endif

LDFLAGS += $(SDL_LDFLAGS)
LDLIBS  += $(SDL_LDLIBS) -lcomdlg32

# --- Main target ---
all: $(TARGET)

$(TARGET): main.cpp $(HELPER_OBJS) $(DRAWING_LIB_OBJS)
ifeq ($(OS),Windows_NT)
	@if not exist "$(BUILD_DIR)" mkdir "$(BUILD_DIR)"
else
	@mkdir -p $(BUILD_DIR)
endif
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) -o $@ main.cpp $(HELPER_OBJS) $(DRAWING_LIB_OBJS) $(LDLIBS)

# --- Pattern rule for object files ---
$(BUILD_DIR)/%.o: %.cpp
ifeq ($(OS),Windows_NT)
	@if not exist "$(dir $@)" mkdir "$(dir $@)"
else
	@mkdir -p $(dir $@)
endif
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

# --- Drawing library ---
# Built by the Pattern rule
# Drawing-Library:
# 	$(MAKE) -C SDL-Drawing-Library

# --- Include auto-generated dependency files ---
-include $(HELPER_OBJS:.o=.d)

# --- Clean ---
clean:
	$(RM) $(TARGET)

cleanAll:
ifeq ($(OS),Windows_NT)
	-@$(RMDIR) "$(BUILD_DIR)"
	-@$(RM) $(TARGET)
else
	$(RMDIR) $(BUILD_DIR)
	$(RM) $(TARGET)
endif

.PHONY: clean cleanAll