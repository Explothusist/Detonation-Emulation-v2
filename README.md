# Detonation-Emulation-v2

Detonation Emulation v2 is a GameBoy emulator for Windows written in C++ using SDL3. It is based on Detonation Emulation (Explothusist/Gameboy-Detonation), a JavaScript emulator. The emulator is a work in progress and may not be usable for 6-12 months.

## Running the Emulator

Go to Releases and download the compiled binary (or follow the instructions further below for building the project). Note that the emulator only has accidental compatability with operating systems other than Windows.

Unzip the file and place the contents in a location of choice. Run the DetonationEmulation.exe file to open the emulator. The EXE file is unsigned, and will cause a Windows SmartScreen warning. Click More Info -> Run Anyway. The first time you run the emulator, it will create two folders (data/ and saves/) for itself in the directory.

## Updating the Emulator

To update to the most recent release, download the new release and copy the data/ and saves/ folders from the old release into the new release.

## Building the Source Code

To build, you will need SDL3, SDL3_image, and SDL3_ttf. When downloading releases, use `<library>-devel-3.X.X-mingw.zip` for MinGW and (standard) Clang on Windows, `<library>-devel-3.X.X-mingw.tar.gz` for MinGW and Clang on Linux, or `<library>-devel-3.X.X-VC.zip` for MSVC or MSVC Clang on Windows.

To build on Windows, I recommend downloading GNUWin32. See below for instructions.

#### Downloading GNUWin32

"make is a GNU command, so the only way you can get it on Windows is installing a Windows version like the one provided by GNUWin32. Anyway, there are several options for getting that:

    - Directly download from Make for Windows  
    - Using Chocolatey. First, you need to install this package manager. Once installed, you simply need to install make (you may need to run it in an elevated/administrator command prompt):  
        - choco install make  
    - Another recommended option is installing a Windows Subsystem for Linux (WSL or WSL 2), so you'll have a Linux distribution of your choice embedded in Windows 10, where you'll be able to install make, gcc, and all the tools you need to build C programs.  
    - For older Windows versions (Microsoft Windows 2000, Windows XP, Windows Server 2003, Windows Vista, Windows Server 2008, or Windows 7 with msvcrt.dll) you can use GnuWin32.

An outdated alternative was MinGW, but the project seems to be abandoned, so it's better to go for one of the previous choices."

— Eduardo Yáñez Parareda, Stack Overflow answer to
“How can I install and use "make" in Windows?”
Original: https://stackoverflow.com/questions/32127524/how-can-i-install-and-use-make-in-windows
Archive: https://web.archive.org/web/20251230160351/https://stackoverflow.com/questions/32127524/how-can-i-install-and-use-make-in-windows
Licensed under CC BY-SA 4.0

### MinGW/Clang

1. Go to https://github.com/libsdl-org/SDL/releases/tag/release-3.4.0 and download the SDL3 release 3.4.0. See above for exactly which file to download.
2. Extract the folder and copy the `SDL3-devel-3.4.0-mingw/SDL3-3.4.0/x86_64-w64-mingw32` (or `/i686-w64-mingw32` for 32-bit computers) folder contents (`bin`, `include`, `lib`, `share`) into `SDL-Drawing-Library/SDL3`.
3. Copy `SDL-Drawing-Library/SDL3/bin/SDL3.dll` (newly created) into the main project folder.

4. Go to https://github.com/libsdl-org/SDL_image/releases/tag/release-3.2.6 and download the SDL3_image release 3.2.6. See above for exactly which file to download.
5. Extract the foler and copy the `SDL3_image-devel-3.2.6-mingw/SDL3_image-3.2.6/x86_64-w64-mingw32` (or `/i686-w64-mingw32/` for 32-bit computers) folder contents (`bin`, `include`, `lib`, `share`) into `SDL-Drawing-Library/SDL3_image`.
6. Copy `SDL-Drawing-Library/SDL3_image/bin/SDL3_image.dll` (newly created) into the main project folder.

7. Go to https://github.com/libsdl-org/SDL_ttf/releases/tag/release-3.2.2 and download the SDL_ttf release 3.2.2. See above for exactly which file to download.
8. Extract the foler and copy the `SDL_ttf-devel-3.2.2-mingw/SDL_ttf-3.2.2/x86_64-w64-mingw32` (or `/i686-w64-mingw32/` for 32-bit computers) folder contents (`bin`, `include`, `lib`, `share`) into `SDL-Drawing-Library/SDL_ttf`.
9. Copy `SDL-Drawing-Library/SDL_ttf/bin/SDL_ttf.dll` (newly created) into the main project folder.

10. Finally, open Command Prompt (or applicable terminal) in the main project folder and run `make BUILD=release CXX=g++` (or `CXX=clang++` for Clang). Run the `DetonationEmulation.exe` file to run the emulator.

### MSVC (Visual Studio / MSVC Clang)

These instructions apply to Visual Studio, MSVC (cl.exe), and MSVC Clang (clang-cl).
Do not use MinGW or GNU Make for this section.

Requirements:  
Visual Studio 2022 (Desktop development with C++)  
OR MSVC Build Tools  
Windows SDK installed  

1. Go to https://github.com/libsdl-org/SDL/releases/tag/release-3.4.0  
2. Download `SDL3-devel-3.4.0-VC.zip`  
3. Extract the archive and copy the contents of `SDL3-3.4.0/` into `SDL-Drawing-Library/SDL3/`  
4. Then copy the correct DLL into the main project folder:  
    - 64-bit: `SDL-Drawing-Library/SDL3/lib/x64/SDL3.dll`  
    - 32-bit: `SDL-Drawing-Library/SDL3/lib/x86/SDL3.dll ` 

5. Go to https://github.com/libsdl-org/SDL_image/releases/tag/release-3.4.0  
6. Download `SDL3_image-devel-3.4.0-VC.zip`  
7. Extract the archive and copy the contents of `SDL3_image-3.4.0/` into `SDL-Drawing-Library/SDL3_image/`  
8. Then copy the correct DLL into the main project folder:  
    - 64-bit: `SDL-Drawing-Library/SDL3_image/lib/x64/SDL3_image.dll`  
    - 32-bit: `SDL-Drawing-Library/SDL3_image/lib/x86/SDL3_image.dll`  

9. Go to https://github.com/libsdl-org/SDL_ttf/releases/tag/release-3.4.0  
10. Download `SDL3_ttf-devel-3.4.0-VC.zip`  
11. Extract the archive and copy the contents of `SDL3_ttf-3.4.0/` into `SDL-Drawing-Library/SDL3_ttf/`  
12. Then copy the correct DLL into the main project folder:  
    - 64-bit: `SDL-Drawing-Library/SDL3_ttf/lib/x64/SDL3_ttf.dll`  
    - 32-bit: `SDL-Drawing-Library/SDL3_ttf/lib/x86/SDL3_ttf.dll`  

#### Building with MSVC

##### Option 1: Visual Studio (recommended)

1. Open **Visual Studio**
2. Create or open the project/solution
3. Set the configuration to:
   - **Release**
   - **x64** (or **Win32**)

4. Add include directories:

    `SDL-Drawing-Library/SDL3/include`  
    `SDL-Drawing-Library/SDL3_image/include`  
    `SDL-Drawing-Library/SDL3_ttf/include`  

5. Add library directories:

    `SDL-Drawing-Library/SDL3/lib/x64`  
    `SDL-Drawing-Library/SDL3_image/lib/x64`  
    `SDL-Drawing-Library/SDL3_ttf/lib/x64`  

6. Link against the following libraries: `SDL3.lib`, `SDL3_image.lib`, `SDL3_ttf.lib`
7. Build the project and run `DetonationEmulation.exe`

##### Option 2: MSVC Clang (advanced)

If you are using **`clang-cl`**, ensure that:

- The target is `windows-msvc`
- You link against `.lib` files
- You **do not** use `make`

Example (simplified):

    `clang-cl /std:c++20 *.cpp SDL3.lib SDL3_image.lib SDL3_ttf.lib`

## Links

Explothusist/Gameboy-Detonation: https://github.com/Explothusist/Gameboy-Detonation

Explothusist/SDL-Drawing-Library: https://github.com/Explothusist/SDL-Drawing-Library
