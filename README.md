# The Great Escape (Group 7)
**Summary:** The puzzle game centers around a group of thieves stealing from different locations, trying to get the art from the your location to their getaway vehicle.

Build a route from your location and collect special abilities on the way to help ward off the officials. Steal from all locations to become the greatest band of thieves alive!

## About
**Team Lead:** Linda Wu

**Group Members:** Michael Darr, Alex Day, Zach Greenfield

**Genre:** Puzzle

**Platform:** Windows, Max OS, Linux

## Compiling
The Great Escape uses [SFML 2](https://www.sfml-dev.org/) for graphics, audio and input, [Box2D](https://box2d.org/) for physics, and [tinyxml2](https://github.com/leethomason/tinyxml2) to read level files. None of them is stored in this repository; CMake fetches them for you:

- **Box2D** v2.4.1 is always downloaded and built.
- **tinyxml2** 11.0.0 is always downloaded and built.
- **SFML**: an installed SFML 2.5+ is used if one is found; otherwise SFML 2.6.2 is downloaded and built (statically linked).

### Requirements
- CMake 3.14 or newer
- A C++11 compiler: Visual Studio 2019+ (Windows), GCC or Clang (Linux/macOS)
- Git and network access the first time you configure (downloads are cached in the build directory afterwards)
- **Linux only:** SFML's system libraries (see below)

### Windows
From a Developer PowerShell / Command Prompt, or open the folder directly in Visual Studio:

```sh
cmake -S . -B build
cmake --build build --config Debug      # or --config Release
build\Debug\greatEscape.exe
```

`openal32.dll` (needed for audio) is copied next to the executable automatically.

### Linux
CMake cannot install system packages, so install the libraries SFML depends on first. On Debian/Ubuntu:

```sh
sudo apt install build-essential cmake git pkg-config \
  libx11-dev libxrandr-dev libxcursor-dev libgl1-mesa-dev libudev-dev \
  libfreetype-dev libopenal-dev libflac-dev libogg-dev libvorbis-dev
```

(Alternatively `sudo apt install libsfml-dev` and the installed SFML is used instead of building it.) If anything is missing, configuring stops with the list of packages to install. Then:

```sh
cmake -S . -B build
cmake --build build -j"$(nproc)"
./build/greatEscape
```

### Build types
On Linux/macOS (Makefiles, Ninja) the build type is chosen at configure time, in this order:

1. `-DCMAKE_BUILD_TYPE=Release` (or `Debug`) if given
2. otherwise from the build directory's name: `release` -> Release, `profile` -> Debug with `-pg` profiling, anything else -> Debug

So `cmake -S . -B release` gives a Release build. Visual Studio builds pick the configuration with `--config` at build time instead.

### Running
The game finds its `data/` directory on its own, so it can be started from any working directory. A development build loads assets straight from this repository's `data/` folder, so edits to assets take effect without rebuilding.

### Making a distributable copy

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
cmake --install build --config Release --prefix GreatEscape
```

This creates a self-contained `GreatEscape/` folder that can be moved or zipped:

- **Windows:** `greatEscape.exe`, `openal32.dll` and `data/`
- **Linux:** `bin/greatEscape` and `share/greatescape/`. The target machine still needs SFML's runtime libraries (X11, OpenGL, OpenAL, FLAC, Vorbis, FreeType, udev), which most desktop distributions already have.

The game locates its data relative to the executable, so the folder works wherever it is placed.

### CMake options

| Option | Default | Description |
|---|---|---|
| `CMAKE_BUILD_TYPE` | from build dir name | `Debug` or `Release` (single-config generators) |
| `SFML_FORCE_FETCH` | `OFF` | Ignore any installed SFML and always build 2.6.2 |
| `GE_INSTALL_BINDIR` | `.` (Windows), `bin` (Linux) | Where `cmake --install` puts the executable, relative to the prefix |
| `GE_INSTALL_DATADIR` | `data` (Windows), `share/greatescape` (Linux) | Where `cmake --install` puts the assets, relative to the prefix |
| `GE_DATA_DIR` | `<repo>/data` | Asset directory used by development (not installed) builds |

### Troubleshooting
- **"SFML was not found, and building it from source needs these missing libraries"** (Linux): install the packages the message lists, then configure again.
- **No sound on Windows**: make sure `openal32.dll` is next to `greatEscape.exe`.
- **No sound under WSL**: audio support in WSL is limited; the game still runs.
- **Changed options not taking effect**: delete the build directory and configure again.

## How To Play
The Great Escape is a physics-based puzzle game that requires the player to figure out a solution to get the stolen object from its original location into the getaway vehicle.

**Gameplay**

![title](/data/ReadMeGuide/TitleScreen.png)

Click play to go to level select screen.

![levelselect](/data/ReadMeGuide/LevelSelect.png)

Click on the glowing orb to select the level that you'd like to play. While levels are not numbered like a traditional level selection page, previous levels that the player has passed will dim to an orange color. The next level unlocked is always a bright yellow color.

![levelone](/data/ReadMeGuide/LevelOneSetup.png)

Click and drag a platform from the platform bank on the right to the desired location. You can use as many or as little of the platforms that you think you need to complete the level. Click play to lock in the platform locations and test your solution. Once you click play, you cannot change your platform's locations.

![levelone](/data/ReadMeGuide/LevelOnePlay.png)

If your solution is successful in getting the stolen object into the getaway van without losing all of the object's durability, then you have successfully compelted the level. The next level will not unlock until you have completed the previous level.

## Project Layout

```
src/
  main.cpp      entry point and main loop
  logic/        game state, levels, physics objects, input handling, menus
  view/         rendering and music (GameView, MenuView)
  resources/    asset loading and data-path lookup
data/           fonts, textures, audio, level and dialogue XML
```

Headers sit next to their sources and are included by module path, e.g. `#include "logic/Level.h"`. Code in `logic/` doesn't include anything from `view/`. Adding a `.cpp` file anywhere under `src/` adds it to the build automatically.

## More Information
For more information about The Great Escape, please refer to our design document [here](https://docs.google.com/document/d/1-G_kxFBZOB9qA2d1C6jzQYF8fPCAyNPuF-_koAcvBis/edit?usp=sharing).
