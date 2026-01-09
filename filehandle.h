#ifndef EMULATOR_FILE_HANDLING_
#define EMULATOR_FILE_HANDLING_

#include <windows.h>
#include <vector>
#include <string>
#include <cstdint>

class Emulator_Options;

std::vector<std::string>* readRecentGamesFile();
void saveRecentGamesFile(std::vector<std::string>* recent_games);

std::vector<uint32_t>* readKeybindingsFile();
void saveKeybindingsFile(std::vector<uint32_t>* keybindings);

Emulator_Options* readOptionsFile();
void saveOptionsFile(Emulator_Options* options);

std::string openROMFilePicker(HWND window_handle);
std::vector<uint8_t>* readROMFile(std::string path);

std::string trimPathAndLength(std::string path, int max_length);

#endif