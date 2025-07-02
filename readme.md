# Gomoku - Layered Architecture Implementation

A demonstration of layered architecture principles through the implementation of the board game [Gomoku](https://en.wikipedia.org/wiki/Gomoku)

## Overview

This project serves as a practical illustration of layered architecture (n-tiered architecture) using a complete Gomoku game implementation. The game showcases how to properly structure applications using distinct layers with clear separation of concerns and controlled inter-layer communication.

## Game Description

Gomoku is a two-player strategy board game where:
- Players alternate placing black and white pieces on a 15×15 grid
- Black always moves first
- The objective is to align five pieces of your color in a row (horizontal, vertical, or diagonal)
- The first player to achieve five in a row wins

## Architecture

The project implements a **3-layer architecture**:

### 1. Presentation Layer
- **Renderer**: Custom OpenGL-based graphics rendering
- **UI Manager**: Handles window events and user interactions
- **Views & Components**: Manages UI hierarchy including MenuViews, BoardView, Labels, and Buttons
- **Menu Controller**: Controls menu visibility and navigation

### 2. Logic Layer
- **Game Service**: Core game management, move validation, and win condition checking
- **Board**: Maintains current game state and move history
- **Player**: Handles both human and AI player logic

### 3. Persistence Layer
- **File Manager**: Handles save/load operations using JSON serialization
- **File System Integration**: Stores game state persistently on disk

## Technologies Used

- **Language**: C++17
- **Graphics**: OpenGL with custom renderer
- **Windowing**: [GLFW](https://www.glfw.org/) (cross-platform window creation)
- **OpenGL Loading**: [GLEW](https://glew.sourceforge.net/)
- **Mathematics**: [GLM](https://github.com/g-truc/glm) (OpenGL Mathematics)
- **Data Format**: [JSON](https://github.com/nlohmann/json) (nlohmann/json library)
- **Image Loading**: [stb_image](https://github.com/nothings/stb)

## Building and Running

### Prerequisites
- C++ compiler with C++17+ support
- OpenGL 4.2+ compatible graphics card
- CMake 3.25

### Linux
```bash
# Clone the repository
git clone --recursive https://github.com/MarQMark/Gomoku
cd Gomoku

# If you forgot --recursive, update submodules manually
git submodule update --init --recursive

# Create build directory
mkdir build && cd build

# Configure and build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release

# Run the game
cd src
./Gomoku
```

### Windows
Open the repository in Visual Studio or CLion and build it from there 

# Screenshots
