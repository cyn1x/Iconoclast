# Overview

Iconoclast is a game engine currently being written in C++.

## Installation

Build support is available for Microsoft Visual Studio 2022 and text editors using `tools\build.bat` if preferred.

### IDE

Opening the Project Solution `(.sln)` file in Visual Studio will automatically configure the IDE. For use with text editors, follow the instructions below to be able to make use of the `build.bat` build script.

Iconoclast is currently only supported on Windows though I hope to add support some time in the future when I would like to take a break from graphics programming.

### Neovim

The project is mainly developed using Neovim with the following dependencies.

- Microsoft Visual Studio 2022
  - Desktop Development for C++
  - Game Development with C++
  - C++ Clang Compiler for Windows (17.0.3)
- Microsoft Visual Studio 2022 Build Tools

#### Configurations

- Add `devenv` to the PATH to debug via the command-line interface.
  - `C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE`
- Add `clang++` to the PATH 
  - `C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\Llvm\x64\bin`

#### Debugging

Run the command below to open the executable in Visual Studio.

```cmd
devenv bin/iconoclast.exe
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

[TheCherno](https://www.thecherno.com/)
[HandmadeHero](https://mollyrocket.com/)
[Learn OpenGL](https://learnopengl.com/)
[RasterTek](https://www.rastertek.com/)

