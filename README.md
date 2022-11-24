# Cellular Automaton

[Conway's game of life](https://en.wikipedia.org/wiki/Conway's_Game_of_Life) written in C++ and [SDL2](https://www.libsdl.org/)

## Demo
![](https://i.imgur.com/aPlyADp.gif)

## Build Instructions

- Requirements
  - Git
  - CMake
  - C++ Compiler

- Steps

    ```cmd
    # Clone this project along with submodules
    $ git clone --recursive https://github.com/AbhayJoshi-Dev/Cellular-Automaton

    # Go to project directory
    $ cd Cellular-Automaton

    # Pull SDL and SDL_image as submodule (if you cloned without --recursive flag)
    $ git submodule update --init

    # Create a folder for build
    $ mkdir build

    # Go to build folder
    $ cd build

    # Generate project files
    $ cmake ..
    ```

- This will generate project files using default cmake generator for your platform. For example:

  - on `Windows` with `Visual Studio` installed, it will generate a `.sln`
  - on `Linux` with `make` installed, it will generate a `Makefile`
  - on `Mac` with `xcode` installed, it will generate a `.xcodeproj`