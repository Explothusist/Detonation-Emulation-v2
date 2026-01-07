#ifndef EMULATOR_FILE_HANDLING_
#define EMULATOR_FILE_HANDLING_

#include <windows.h>
#include <vector>
#include <string>

std::vector<std::string>* readRecentGamesFile();
void saveRecentGamesFile(std::vector<std::string>* recent_games);

std::string openROMFilePicker(HWND window_handle);

#endif