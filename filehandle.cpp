
#include "filehandle.h"
#include <windows.h>
#include <commdlg.h>
#include <iostream>
#include <fstream>
#include <filesystem>

std::vector<std::string>* readRecentGamesFile() {
    printf("readRecentGamesFile\n");
    std::vector<std::string>* recent_games{ new std::vector<std::string>() };

    std::ifstream f_recent_games;
    f_recent_games.open("data/recent_games.txt");

    if (!f_recent_games.is_open()) {
        printf("WARNING: Initialization: File 'data/recent_games.txt' Not Found\n");

        std::ofstream create_f_recent_games;
        create_f_recent_games.open("data/recent_games.txt");
        if (!create_f_recent_games.is_open()) {
            // printf("ERROR: Initialization: Could Not Creat File 'data/recent_games.txt'\n");

            std::filesystem::create_directory("data");
            printf("COMPLETE: Initialization: Created Directory 'data'\n");
            create_f_recent_games.open("data/recent_games.txt");
        }

        for (int i = 0; i < 5-1; i++) {
            create_f_recent_games << "\n";
        }
        for (int i = 0; i < 5; i++) {
            recent_games->push_back("");
        }
        create_f_recent_games.close();
        printf("COMPLETE: Initialization: Created File 'data/recent_games.txt'\n");
    }else {
        printf("file exists\n");
        std::string line;
        while (getline(f_recent_games, line)) {
            printf("reading line\n");
            printf((line+"\n").c_str());
            recent_games->push_back(line);
        }
        f_recent_games.close();
    }

    return recent_games;
};

std::string openROMFilePicker(HWND window_handle) {
    OPENFILENAME ofn;       // Common dialog box structure
    char szFile[260];    // Buffer for file name

    // Initialize OPENFILENAME
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn); // Must be initialized with size of the structure
    ofn.hwndOwner = window_handle;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);  // Maximum length of the file path
    ofn.lpstrFilter = "GameBoy ROM Files (*.gb)\0*.GB\0All Files (*.*)\0*.*\0";  // Filter for .gb files
    ofn.nFilterIndex = 1;  // Start with the first filter (GameBoy ROM files)
    ofn.lpstrFileTitle = NULL; // We don't need the file name separately
    ofn.nMaxFileTitle = 0;  // Not needed here
    ofn.lpstrInitialDir = NULL;  // No specific initial directory
    ofn.lpstrTitle = "Open GameBoy ROM File";  // Title of the dialog box
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;  // Ensure valid file selection

    // Display the Save As dialog box
    if (GetOpenFileName(&ofn) == TRUE) {
        std::cout << "Selected file: " << ofn.lpstrFile << std::endl;
        return ofn.lpstrFile;
    } else {
        std::cout << "No file selected or operation canceled." << std::endl;
        return "";
    }
};
