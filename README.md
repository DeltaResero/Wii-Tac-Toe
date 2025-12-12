# Wii-Tac-Toe

<p align="center">
  <img src="http://wiibrew.org/w/images/e/e7/Wii-Tac-Toe-screenshot.png" alt="Wii-Tac-Toe Screenshot" width="600">
</p>

<br>

## A Tic-Tac-Toe game for the Nintendo Wii

```text
Original Author:  Crayon
Graphics Artist:  Mr_Nick666
Modifications by: DeltaResero
Type:             Puzzle / Board Game
License:          MIT (Code Only)
```
[![Latest Release][release-img]][release-url]
[![View All Releases][downloads-img]][downloads-url]
[![License][license-img]][license-url]
[![Codacy Badge][codacy-img]][codacy-url]

<!-- Link Definitions -->
[release-img]: https://img.shields.io/github/v/release/DeltaResero/Wii-Tac-Toe?label=Latest%20Release
[release-url]: https://github.com/DeltaResero/Wii-Tac-Toe/releases/latest
[downloads-img]: https://img.shields.io/badge/Downloads-View_All_Releases-blue
[downloads-url]: https://github.com/DeltaResero/Wii-Tac-Toe/releases
[license-img]: https://img.shields.io/badge/license-MIT-blue.svg
[license-url]: https://github.com/DeltaResero/Wii-Tac-Toe/blob/master/LICENSE
[codacy-img]: https://app.codacy.com/project/badge/Grade/4e2a8a80cf1948a59208e585339c2b69
[codacy-url]: https://app.codacy.com/gh/DeltaResero/Wii-Tac-Toe/dashboard?utm_source=gh&utm_medium=referral&utm_content=&utm_campaign=Badge_grade

<br>

### About Repository

This is a fork of the [original Wii-Tac-Toe](https://github.com/Crayon2000/Wii-Tac-Toe) Wii homebrew game by **Crayon**,
designed for use through the [Homebrew Channel](http://wiibrew.org/wiki/Homebrew_Channel). This fork serves as a modernized
codebase, updating the build system to CMake and ensuring compatibility with the latest devkitPro toolchains. It also
includes minor gameplay tweaks and code refactoring to improve maintainability.

The source code for this project is licensed under the **MIT License**. The graphical assets were created by **Mr_Nick666**.
These assets remain **All Rights Reserved** and are not covered by the [MIT license](https://opensource.org/license/mit).
Please treat all images and binary assets as proprietary until further notice. In the future, this fork plans to replace the
assets with its own versions to set it apart from the original with clearer licensing.

<br>

### About Wii-Tac-Toe

Wii-Tac-Toe is a classic Tic-Tac-Toe game programmed in C++ using devkitPro and GRRLIB.

This game features both solo play against the CPU and a two player mode, either by sharing one Wii Remote or using two.
The game’s AI varies its moves so that games feel more unique and challenging.

To win, align three of your marks (O or X) in a horizontal, vertical, or diagonal row. If all spaces are filled without
a winner, it’s a tie. Start a new game and "Wii"-ignite the fun!

<br>

### Setup Guide for devkitPro PowerPC Build System

To build Wii-Tac-Toe, you'll need the **devkitPro devkitPPC** toolchain installed. To set up the devkitPro devkitPPC
PowerPC build system, follow the instructions on the official devkitPro wiki:

- [Getting Started with devkitPro](https://devkitpro.org/wiki/Getting_Started)
- [devkitPro Pacman](https://devkitpro.org/wiki/devkitPro_pacman)

After setting up devkitPPC including environment variables, use (dkp-)pacman
to install the following dependencies:

```bash
ppc-mxml
ppc-libpng
ppc-freetype
libogc
libfat-ogc
wii-cmake
wii-pkg-config
```

### How to Build: Wii Homebrew Build

1. After cloning this repository, installing the devkitPro devkitPPC toolchain and required dependencies, create a build directory in the root folder:
   ```bash
   mkdir build
   cd build
   ```

2. Configure using CMake and the Wii toolchain file:
   ```bash
   cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/Wii.cmake ..
   ```

3. Compile using GNU Make (-j option to speed up the process)
   ```bash
   make -j$(nproc)
   ```

This will generate `boot.dol` in the build folder.

<br>

### Installation

1. Copy the `Wii-Tac-Toe` folder and its contents (`icon.png` and `meta.xml`) from the source root to the `apps` directory on your SD card or USB drive (e.g., `SD:/apps/Wii-Tac-Toe`).

2. Copy the compiled `boot.dol` to that folder.

3. Insert the SD card or USB drive into your Wii and launch via the Homebrew Channel.

<br>

### Disclaimer

This is an unofficial homebrew application that runs on the Wii via the Homebrew Channel. It is not affiliated with,endorsed by, nor sponsored
by the creators of the Wii console nor the Homebrew Channel. All trademarks and copyrights are the property of their respective owners.

This project is distributed in the hope that it will be useful, but **WITHOUT ANY WARRANTY**; without even the implied warranty of **MERCHANTABILITY** or **FITNESS FOR A PARTICULAR PURPOSE**.
