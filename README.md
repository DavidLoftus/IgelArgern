# IgelArgern
Igel Ärgern game written in C

## Dependencies
CMake is used to create build files, make sure you have installed it using the instructions on their website https://cmake.org/download/

This application also uses the curses library, if you using Linux you can install it via the command:
```
sudo apt-get install libncurses-dev
```
On Windows, please use the MinGW compiler, you can install them and the ncurses library on the mingw-get installer. There is a few issues with this version of the library so Linux is the preferred platform.
Note: ncurses is not available on the MSVC toolchain and we have not added support for PDCurses.

## Build steps
```
git clone https://github.com/DavidLoftus/IgelArgern # Clone repository
cd IgelArgern
mkdir build # Create build folder
cd build
cmake .. # Create cmake build files
cmake --build . # Automatically runs build tools, can also be opened in IDE
```

