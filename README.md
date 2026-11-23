# The Great Escape (Group 7)
**Summary:** The puzzle game centers around a group of thieves stealing from different locations, trying to get the art from the your location to their getaway vehicle.

Build a route from your location and collect special abilities on the way to help ward off the officials. Steal from all locations to become the greatest band of thieves alive!

## About
**Team Lead:** Linda Wu

**Group Members:** Michael Darr, Alex Day, Zach Greenfield

**Genre:** Puzzle

**Platform:** Windows, Max OS, Linux

## Compiling
The Great Escape utilizes SFML 2 and Box2D. CMake 3.14+ is required, and the first configure needs network access.

- **Box2D** (v2.4.1) is always downloaded and built by CMake.
- **SFML**: if an SFML 2.5+ install is found it is used; otherwise SFML 2.6.2 is downloaded and built automatically (pass `-DSFML_FORCE_FETCH=ON` to always build it).

```sh
cmake -S . -B build
cmake --build build
```

**Linux:** building SFML from source needs system development packages that CMake cannot install. Configure stops with a list of anything missing. On Debian/Ubuntu either install SFML itself (`sudo apt install libsfml-dev`) or its dependencies:

```sh
sudo apt install build-essential cmake git pkg-config \
  libx11-dev libxrandr-dev libxcursor-dev libgl1-mesa-dev libudev-dev \
  libfreetype-dev libopenal-dev libflac-dev libogg-dev libvorbis-dev
```

The built game finds its assets on its own, so it can be run from any directory.

### Making a distributable copy

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
cmake --install build --config Release --prefix GreatEscape
```

This creates a self-contained `GreatEscape/` folder that can be moved or zipped:

- **Windows:** `greatEscape.exe`, `openal32.dll` and `data/`
- **Linux:** `bin/greatEscape` and `share/greatescape/` (the system libraries SFML uses, e.g. X11/OpenGL/OpenAL, must be present on the target machine)

The layout can be changed with `-DGE_INSTALL_BINDIR=...` and `-DGE_INSTALL_DATADIR=...` (both relative to the prefix).

SFML: https://www.sfml-dev.org/download.php

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

## More Information
For more information about The Great Escape, please refer to our design document [here](https://docs.google.com/document/d/1-G_kxFBZOB9qA2d1C6jzQYF8fPCAyNPuF-_koAcvBis/edit?usp=sharing).
