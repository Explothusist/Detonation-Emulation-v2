# Detonation-Emulation-v2

Detonation Emulation v2 is a GameBoy emulator written in C++ using SDL3. It is based on Detonation Emulation (Explothusist/Gameboy-Detonation), a JavaScript emulator. The emulator is a work in progress and may not be usable for 6-12 months.

The DetonationEmulation.exe file is unsigned, and will cause a Windows SmartScreen warning. Click More Info -> Run Anyway.

To update to the most recent release, download the new release and copy the data/ and saves/ folders from the old release into the new release. May not work for all releases, so check the release in question.

The emulator uses Explothusist/SDL-Drawing-Library, which wraps SDL in an easy to use framework for drawing basic graphics based on the JavaScript Canvas.

SDL-Drawing-Library requires SDL3 to build. Detonation Emulation v2 relies on the library's version of SDL3, but requires the .dll to run. Specifically, it needs SDL3_image.dll, SDL3_ttf.dll, and SDL3.dll. These .dll files are provided in the releases. See SDL-Drawing-Library for more information.

Explothusist/Gameboy-Detonation: https://github.com/Explothusist/Gameboy-Detonation

Explothusist/SDL-Drawing-Library: https://github.com/Explothusist/SDL-Drawing-Library
