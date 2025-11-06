# Task Manager

A simple desktop task manager application built with ImGui, SDL2, and OpenGL.

## Features

- Add and delete tasks
- Mark tasks as complete/incomplete
- Simple, clean UI

## Prerequisites

- CMake 3.15+
- C++17 compiler
- SDL2 development libraries
- OpenGL development libraries

## Setup

### 1. Install Dependencies (macOS)

```bash
brew install sdl2 pkg-config
```

### 2. Clone ImGui

```bash
git clone https://github.com/ocornut/imgui.git imgui/imgui
```

### 3. Build

```bash
mkdir build
cd build
cmake ..
make
```

### 4. Run

```bash
./task-manager
```

## Project Structure

```
task-manager/
├── src/
│   └── main.cpp          # Application code
├── CMakeLists.txt        # Build configuration
├── .vscode/              # IDE settings
└── README.md
```

## How It Works

- **ImGui**: UI framework for creating windows and widgets
- **SDL2**: Windowing and event handling
- **OpenGL**: Graphics rendering
