
#include "filehandle.h"
#include <windows.h>
#include <commdlg.h>
#include <iostream>
#include <fstream>
#include <filesystem>

#include "utils.h"

std::vector<std::string>* readRecentGamesFile() {
    std::vector<std::string>* recent_games{ new std::vector<std::string>() };

    std::ifstream f_recent_games; // Input (read) file
    f_recent_games.open("data/recent_games.txt");

    if (!f_recent_games.is_open()) {
        printf("WARNING: Initialization: File 'data/recent_games.txt' Not Found\n");

        for (int i = 0; i < 5; i++) {
            recent_games->push_back("");
        }
        saveRecentGamesFile(recent_games);

        // std::ofstream create_f_recent_games;
        // create_f_recent_games.open("data/recent_games.txt");
        // if (!create_f_recent_games.is_open()) {
        //     // printf("ERROR: Initialization: Could Not Creat File 'data/recent_games.txt'\n");

        //     std::filesystem::create_directory("data");
        //     printf("COMPLETE: Initialization: Created Directory 'data'\n");
        //     create_f_recent_games.open("data/recent_games.txt");
        // }

        // for (int i = 0; i < 5-1; i++) {
        //     create_f_recent_games << "\n";
        // }
        // create_f_recent_games.close();
        printf("COMPLETE: Initialization: Created File 'data/recent_games.txt'\n");
    }else {
        std::string line;
        while (getline(f_recent_games, line)) {
            recent_games->push_back(line);
        }
        for (int i = static_cast<int>(recent_games->size()); i < 5; i++) { // In case the file gets clipped
            recent_games->push_back("");
        }
        f_recent_games.close();
    }

    return recent_games;
};
void saveRecentGamesFile(std::vector<std::string>* recent_games) {
    std::ofstream f_recent_games; // Output (write) file
    f_recent_games.open("data/recent_games.txt", std::ios::trunc);
    if (!f_recent_games.is_open()) {
        // printf("ERROR: Save Data: Could Not Creat File 'data/recent_games.txt'\n");

        std::filesystem::create_directory("data");
        printf("COMPLETE: Save Data: Created Directory 'data'\n");
        f_recent_games.open("data/recent_games.txt", std::ios::trunc);
    }

    for (int i = 0; i < static_cast<int>(recent_games->size()); i++) {
        f_recent_games << recent_games->at(i).c_str();
        if (i != static_cast<int>(recent_games->size()) - 1) {
            f_recent_games << "\n";
        }
    }
    f_recent_games.close();
};

std::vector<uint32_t>* readKeybindingsFile() {
    std::vector<uint32_t>* keybindings{ new std::vector<uint32_t>(8) };

    std::ifstream f_keybindings_file;
    f_keybindings_file.open("data/keybindings.bin", std::ios::binary);

    if (!f_keybindings_file.is_open()) {
        printf("WARNING: Initialization: File 'data/keybindings.bin' Not Found\n");

        keybindings = get_default_keybindings();
        saveKeybindingsFile(keybindings);

        // std::ofstream create_f_keybindings;
        // create_f_keybindings.open("data/keybindings.bin", std::ios::binary);
        // if (!create_f_keybindings.is_open()) {
        //     std::filesystem::create_directory("data");
        //     printf("COMPLETE: Initialization: Created Directory 'data'\n");
        //     create_f_keybindings.open("data/keybindings.bin", std::ios::binary);
        // }

        // // for (int i = 0; i < 8; i++) {
        // //     create_f_keybindings << keybindings.at(i);
        // // }
        // create_f_keybindings.write(
        //     reinterpret_cast<const char*>(keybindings->data()),
        //     8 * sizeof(uint32_t)
        // );
        // create_f_keybindings.close();
        printf("COMPLETE: Initialization: Created File 'data/keybindings.bin'\n");
    }else {
        f_keybindings_file.seekg(0, std::ios::end);
        std::streamsize fileSize = f_keybindings_file.tellg();
        f_keybindings_file.seekg(0, std::ios::beg);

        if (fileSize != 32) {
            printf("WARNING: Initialization: File 'data/keybindings.bin' Is Corrupted\n");

            keybindings = get_default_keybindings();
            saveKeybindingsFile(keybindings);

            // std::ofstream overwrite_f_keybindings;
            // overwrite_f_keybindings.open("data/keybindings.bin", std::ios::trunc | std::ios::binary);

            // keybindings = get_default_keybindings();
            // overwrite_f_keybindings.write(
            //     reinterpret_cast<const char*>(keybindings->data()),
            //     8 * sizeof(uint32_t)
            // );
            // overwrite_f_keybindings.close();
            printf("COMPLETE: Initialization: Repaired File 'data/keybindings.bin'\n");
        }else {
            if (f_keybindings_file.read(reinterpret_cast<char*>(keybindings->data()), 32)) {

            }else {
                printf("ERROR: Initialization: File 'data/keybindings.bin' Could Not Be Read\n");
            }
        }
    }

    // for (int i = 0; i < static_cast<int>(keybindings.size()); i++) {
    //     printf("%i\n", keybindings[i]);
    // }

    return keybindings;
};
void saveKeybindingsFile(std::vector<uint32_t>* keybindings) {
    std::ofstream f_keybindings_file;
    f_keybindings_file.open("data/keybindings.bin", std::ios::trunc | std::ios::binary);
    if (!f_keybindings_file.is_open()) {
        std::filesystem::create_directory("data");
        printf("COMPLETE: Save Data: Created Directory 'data'\n");
        f_keybindings_file.open("data/keybindings.bin", std::ios::trunc | std::ios::binary);
    }

    f_keybindings_file.write(
        reinterpret_cast<const char*>(keybindings->data()),
        8 * sizeof(uint32_t)
    );
    f_keybindings_file.close();
};

Emulator_Options* readOptionsFile() {
    Emulator_Options* options{ nullptr };

    std::ifstream f_options_file;
    f_options_file.open("data/options.txt");

    if (!f_options_file.is_open()) {
        printf("WARNING: Initialization: File 'data/options.txt' Not Found\n");

        options = get_default_options();

        saveOptionsFile(options);

        // std::ofstream create_f_options_file;
        // create_f_options_file.open("data/options.txt");
        // if (!create_f_options_file.is_open()) {
        //     std::filesystem::create_directory("data");
        //     printf("COMPLETE: Initialization: Created Directory 'data'\n");
        //     create_f_options_file.open("data/options.txt");
        // }


        // create_f_options_file << std::boolalpha << options->m_run_boot_rom << "\n";
        // create_f_options_file.close();
        printf("COMPLETE: Initialization: Created File 'data/options.txt'\n");
    }else {
        std::string line;

        bool run_boot_rom = true;
        if (getline(f_options_file, line)) {
            run_boot_rom = (line == "false") ? false : true; // true if broken
        }
        bool strict_loading = false;
        if (getline(f_options_file, line)) {
            strict_loading = (line == "true") ? true : false; // false if broken
        }
        bool display_cart_info = false;
        if (getline(f_options_file, line)) {
            display_cart_info = (line == "true") ? true : false; // false if broken
        }

        options = new Emulator_Options(run_boot_rom, strict_loading, display_cart_info);
        f_options_file.close();
    }

    return options;
};
void saveOptionsFile(Emulator_Options* options) {
    std::ofstream f_options_file;
    f_options_file.open("data/options.txt", std::ios::trunc);
    if (!f_options_file.is_open()) {
        std::filesystem::create_directory("data");
        printf("COMPLETE: Save Data: Created Directory 'data'\n");
        f_options_file.open("data/options.txt", std::ios::trunc);
    }

    f_options_file << std::boolalpha << options->m_run_boot_rom << "\n";
    f_options_file << std::boolalpha << options->m_strict_loading << "\n";
    f_options_file << std::boolalpha << options->m_display_cart_info << "\n";
    f_options_file.close();
};

std::string openROMFilePicker(HWND window_handle) {
    char currentDir[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, currentDir); // GetOpenFileName messes up CWD

    OPENFILENAME ofn;       // Common dialog box structure
    // Buffer for file name
    char szFile[260] = {0};  // or memset(szFile, 0, sizeof(szFile));

    // Initialize OPENFILENAME
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn); // Must be initialized with size of the structure
    ofn.hwndOwner = window_handle;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);  // Maximum length of the file path
    char filter[] = "GameBoy ROM Files (*.gb)\0*.GB\0All Files (*.*)\0*.*\0\0";
    ofn.lpstrFilter = filter;  // Filter for .gb files
    ofn.nFilterIndex = 1;  // Start with the first filter (GameBoy ROM files)
    ofn.lpstrFileTitle = NULL; // We don't need the file name separately
    ofn.nMaxFileTitle = 0;  // Not needed here
    ofn.lpstrInitialDir = NULL;  // No specific initial directory
    ofn.lpstrTitle = "Open GameBoy ROM File";  // Title of the dialog box
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;  // Ensure valid file selection

    // Display the Save As dialog box
    bool success = GetOpenFileName(&ofn);

    SetCurrentDirectory(currentDir); // Repair CWD

    if (success) {
        std::cout << "Selected file: " << ofn.lpstrFile << std::endl;
        return ofn.lpstrFile;
    } else {
        std::cout << "No file selected or operation canceled." << std::endl;
        return "";
    }
};
std::vector<uint8_t>* readROMFile(std::string path) {
    std::vector<uint8_t>* rom{ nullptr };

    std::ifstream f_rom_file;
    f_rom_file.open(path, std::ios::binary);
    if (!f_rom_file.is_open()) {
        printf("ERROR: Read ROM File: Could Not Read File At Filepath '%s'", path.c_str());
        return nullptr;
    }

    f_rom_file.seekg(0, std::ios::end);
    const std::streamsize file_size = f_rom_file.tellg();
    f_rom_file.seekg(0, std::ios::beg);

    rom = new std::vector<uint8_t>(file_size);

    f_rom_file.read(reinterpret_cast<char*>(rom->data()), file_size);

    return rom;
};

std::string trimPathAndLength(std::string path, int max_length) {
    std::string filename = path.substr(path.find_last_of("/\\") + 1);
    if (filename.length() < static_cast<unsigned int>(max_length)) {
        return filename;
    }else {
        return filename.substr(0, max_length-3)+"...";
    }
};
