#ifndef EMULATOR_FILE_HANDLING_
#define EMULATOR_FILE_HANDLING_

#include <windows.h>
#include <vector>
#include <string>
#include <cstdint>

std::vector<std::string>* readRecentGamesFile();
void saveRecentGamesFile(std::vector<std::string>* recent_games);

std::vector<uint32_t>* readKeybindingsFile();
void saveKeybindingsFile(std::vector<uint32_t>* keybindings);

std::string openROMFilePicker(HWND window_handle);

std::string trimPathAndLength(std::string path, int max_length);

#endif