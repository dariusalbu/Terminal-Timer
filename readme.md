# Terminal Timer

A lightweight, cross-platform productivity timer for the terminal. Built with C using `ncurses` for the UI and `miniaudio` for high-quality audio alerts.

## Features

* **Cross-Platform**: Works on Linux, macOS, and Windows.
* **Audio Alerts**: Plays a sound when the timer finishes.
* **Low Overhead**: Minimal CPU and memory usage.

## Compatibility

* **Linux/macOS**: Uses standard `ncurses` and `pthread`.
* **Windows**: Compatible via `PDCurses` and Windows Multimedia (winmm).

## How to Build

### 1. Prerequisites

* **CMake** (version 3.10 or higher)
* **C Compiler** (GCC, Clang, or MSVC)
* **Libraries**:
  * Linux: `libncurses-dev`
    * How to install(Ubuntu/Debian):
      ```bash
      gcc ttimer.c -o ttimer -lncurses -lpthread
      ```
  * Windows: `PDCurses`
    * This project requires PDCurses to run natively on Windows.
    * How to install(using vcpkg):
    1. Install [vcpkg](https://github.com/microsoft/vcpkg)
    2. Install the library:
        ```powershell
        vcpkg install pdcurses:x64-windows
        vcpkg integrate install
        ```

### 2. Build Instructions

Open your terminal/command prompt in the project folder and run:

```bash
# Generate build files
cmake -S . -B build

# Compile the project
cmake --build build
```

## Running the Timer
Before running, ensure your sound file (alarm.wav) is in the same folder as the executable so the audio engine can find it.

### Linux/MacOS

```bash
./build/ttimer
```
### Windows

```powershell
.\build\Debug\ttimer.exe
```