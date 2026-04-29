# Overview

Iconoclast is a game engine currently being written in C++.

## Installation

Build support is available for Microsoft Visual Studio 2026. There is also build support for text editors using `tools\build.bat` to bind to a build command.

Iconoclast is currently only supported on Windows.

### IDE

Opening the Project Solution `(*.slnx)` file in Visual Studio will automatically configure the IDE. For use with text editors, follow the instructions below to be able to make use of the `build.bat` build script.

- Microsoft Visual Studio 2026
  - Workloads
    - Desktop Development for C++
    - Game Development with C++

### Text Editor

The project requires the following dependencies for text editors.

- Microsoft Visual Studio BuildTools 2026
  - Workloads
    - Desktop Development for C++
    - Game Development with C++
  - Individual components
    - C++ Clang Compiler for Windows (20.1.8)
    - MSBuild support for LLVM (clang-cl) toolset

Clang acts as the Language Server Protocal (LSP), and MSBuild is required to compile the project source files.

#### Configurations

- Add `devenv` to the PATH to debug via the command-line interface.
  - `C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE`
- Add `clang++` to the PATH 
  - `C:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\Llvm\x64\bin`

#### Debugging

Run the command below to open the executable in Visual Studio 2026.

```cmd
devenv bin\windows\Debug_x86_64\sandbox.exe
```

Press `F11` to **Step Into** the program entry point, which shows the code being executed for debugging purposes.

## Roadmap

The following serves as a rough outline of the key milestones to indicate the trajectory of the project.

Legend:

&#x2705; public &nbsp;|&nbsp; &#x1F512; private &nbsp;|&nbsp; &#x1F504; in progress &nbsp;|&nbsp; &#x23F3; planned

Current features:

- &#x1F512; Software renderer
- &#x1F512; Keyboard and mouse input
- &#x1F512; <a href="https://learn.microsoft.com/en-us/windows/win32/xinput/xinput-game-controller-apis-portal" target="_blank" rel="noopener noreferrer">XInput</a> gamepad controls
- &#x2705; Event system
- &#x2705; Multi-Graphics API abstraction layer
- &#x1F512; Hot reloading

Future plans:

- &#x1F504; <a href="https://learn.microsoft.com/en-us/windows/win32/direct3d11/atoc-dx-graphics-direct3d-11" target="_blank" rel="noopener noreferrer">DirectX 11</a> renderer
- &#x1F504; OpenGL renderer
- &#x23F3; <a href="https://learn.microsoft.com/en-us/windows/win32/xaudio2/xaudio2-apis-portal" target="_blank" rel="noopener noreferrer">XAudio2</a> audio layer
- &#x23F3; SIMD Mathematics library
- &#x23F3; Memory management system
- &#x23F3; Performance profiling

### Notes

This project changed from C to C++ at SHA `62338d2`, and a fork was created at SHA `41f06e7` to continue development in C. There are currently no plans to port locked features from the forked C project to C++ except for input controls.

The forked C project uses <a href="https://learn.microsoft.com/en-us/previous-versions/windows/desktop/bb318665(v=vs.85)">DirectSound</a> in place of XAudio2, as XAudio2 has no C implementation.

# Mentions

Resources that have helped me, and have continued to help me work on this project.

[TheCherno](https://www.thecherno.com/)<br/>
[HandmadeHero](https://mollyrocket.com/)<br/>
[Learn OpenGL](https://learnopengl.com/)<br/>
[RasterTek](https://www.rastertek.com/)<br/>

