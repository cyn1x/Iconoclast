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

- [x] Software rendering
- [x] Event system
- [ ] Keyboard and mouse input
- [ ] Multi-Graphics API runtime support
- [ ] OpenGL renderer
- [ ] DirectX11 renderer
- [ ] XInput gamepad controls
- [ ] XAudio2 audio layer
- [ ] SIMD mathematics library
- [ ] Memory management system
- [ ] Performance profiling
- [ ] Hot reloading
- [ ] Desktop GUI

# Mentions

Resources that have helped me, and have continued to help me work on this project.

[TheCherno](https://www.thecherno.com/)<br/>
[HandmadeHero](https://mollyrocket.com/)<br/>
[Learn OpenGL](https://learnopengl.com/)<br/>
[RasterTek](https://www.rastertek.com/)<br/>

