@echo off

setlocal enabledelayedexpansion

set err=

set argCount=0
for %%x in (%*) do (
    set /A argCount+=1
    set "argVec[!argCount!]=%%~x"
)

rem Default fallback if MSVC does not automatically set platform
if "%platform%"=="" set platform=x64

set config=Debug
set compileropts=-DDEBUG=1
set linkeropts=/DEBUG

rem Set project root directory
set root=%~dp0..
for %%I in ("%ROOT%") do set "root=%%~fI"

rem Process command-line arguments arguments
for /L %%i in (1,1,%argCount%) do (
    
    set arg=!argVec[%%i]!

    if !arg! EQU clean goto :clean

    if !arg! EQU release (
        set compileropts=-Od -DDEBUG=0
        set linkeropts=/OPT:REF
        set config=Release
    )
    if !arg! EQU x86 (
        rem Override platform set by MSVC if specified via command-line args
        set platform=x86
    )
)

rem End of entry point procedure
goto :main

:clean
del /S /Q Iconoclast\obj\*.* > nul
del /S /Q Iconoclast\bin\*.* > nul
del /S /Q Sandbox\obj\*.* > nul
del /S /Q Sandbox\bin\*.* > nul

rem End of :clean subroutine call
goto :eof

:main

if not defined DevEnvDir (
    call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" %platform%
)

rem Define the engine dynamic link library and application executable filenames
set libfilename=iconoclast_%platform%.lib
set exefilename=Sandbox.exe

rem Define `compile_flags.txt` file for the LLVM Clang LSP
set compileflags=compile_flags.txt
 
rem Define external libraries for linking to the executable
set links=user32.lib gdi32.lib dxgi.lib d3d11.lib d3dcompiler.lib opengl32.lib

rem Clear `compile_flags.txt` file
echo | set /p clear="" > %compileflags%

rem Make required directories for core engine and sandbox environment
call :mkdirs Iconoclast
call :mkdirs Sandbox

goto :build

:mkdirs
rem Make required directories for engine compilation
if not exist %1\obj mkdir %1\obj
if not exist %1\obj\win\%config%_x64 mkdir %1\obj\win\%config%_x64
if not exist %1\obj\win\%config%_x86 mkdir %1\obj\win\%config%_x86
if not exist %1\bin mkdir %1\bin
if not exist %1\bin\win\%config%_x64 mkdir %1\bin\win\%config%_x64
if not exist %1\bin\win\%config%_x86 mkdir %1\bin\win\%config%_x86

rem End of `:mkdirs` subroutine call
goto :eof

rem Builds the entire solution
:build

call :library
if %errorlevel% neq 0 goto :eof
call :executable
if %errorlevel% neq 0 goto :eof
goto :eof

rem ==========================
rem Build Iconoclast (Library)
rem ==========================
:library

rem Set the `include` and `src` directories for the Iconoclast project
set incdir=%root%\Iconoclast\include
set srcdir=%root%\Iconoclast\src

rem Set vendor `include` directories, separated by a space ` ` character
set vendorincs=\Iconoclast\vendor\glad\include

rem Set the `obj` and `bin` directories for the Iconoclast project
set objdir=%root%\Iconoclast\obj\win\%config%_%platform%
set bindir=%root%\Iconoclast\bin\win\%config%_%platform%

rem Set the `*.lib` library file for the Iconoclast engine
set libfilepath=%bindir%\%libfilename%

rem Clear variables to store source, include, and object filenames.
set srcs=
set incs=
set objs=

call :include
call :vendor
call :sources
goto :compile

rem Set all relative include directory paths
:include
rem Reference the absolute and relative paths of each `*.h` header file
for /r %incdir% %%F in (*.h) do (

    call :relpath %%~dpF

    rem Prevent duplicate include directories
    if !prev! neq !relpath! (

        call set "incs=%%incs%% -I%root%!relpath:~0,-1!"
        call :addcompileflag !relpath!

        set "prev=!relpath!"
    )
)

rem End of `:include` subroutine call
goto :eof

rem Set all vendor include directory paths
:vendor
rem Adds custom vendor include directories to append to `compile_flags.txt`

rem Append each directory to compiler includes, and `compile_flags.txt`
for %%D in (%vendorincs%) do (
    
    set "dir=%%D"
    
    call set "incs=%%incs%% -I%root%!dir!"
    call :addcompileflag !dir!
)

rem End of `:vendor` subroutine call
goto :eof

rem Recursively set all relative file paths of `*.cpp` source files
:sources
for /r %srcdir% %%F in (*.cpp) do (

    if %%~nF neq IconoclastPCH (
        call :relpath %%F
        call set "srcs=%%srcs%% %root%\!relpath!"
    )
)

rem End of `:sources` subroutine call
goto :eof

:compile
cl /EHsc /c /std:c++20 /MDd /FAsc /Zi /WX /W4 /Yc"IconoclastPCH.h" /Fo%objdir%\ /Fp"%objdir%\IconoclastPCH.pch" /Fd"%objdir%\vc140.pdb" /Fa"%objdir%\IconoclastPCH.asm" %incs% %srcdir%\IconoclastPCH.cpp
if %errorlevel% neq 0 goto :error

cl /nologo /EHsc /c /std:c++20 /JMC /MDd /FAsc /Zi /Yu"IconoclastPCH.h" /Fp"%objdir%\IconoclastPCH.pch" /WX /W4 -wd4201 -wd4100 -wd4189 -wd4505 %compileropts% /Fo%objdir%\ /Fd"%objdir%\vc140.pdb" /Fa%objdir%\ %incs% %srcs:~1%
if %errorlevel% neq 0 goto :error
echo Engine source files compiled successfully & echo.

rem Store all *.obj file names only for the linker
for /r %objdir% %%F in (*.obj) do (
    call set "objs=%%objs%% %objdir%\%%~nxF"
)

echo Creating library...
LIB /SUBSYSTEM:WINDOWS %objs:~1% /OUT:%libfilepath%
if %errorlevel% neq 0 goto :error
call :relpath %bindir%
echo Engine library file `%libfilename%` created in `%relpath%` successfully. & echo.

goto :eof

rem ==========================
rem Build Sandbox (Executable)
rem ==========================
:executable

set includedir=%root%\Sandbox\include

rem Set the `include` and `src` directories for the Sandbox project
set srcdir=%root%\Sandbox\src

rem Set the `obj` and `bin` directories for the Sandbox project
set objdir=%root%\Sandbox\obj\win\%config%_%platform%
set bindir=%root%\Sandbox\bin\win\%config%_%platform%

rem Set the `*.exe` executable file for the Sandbox application
set exepath=%root%\Sandbox\bin\win\%config%_%platform%\%exefilename%

rem Clear `srcs` and `objs` variables to store new source and object filenames
set srcs=
set objs=

rem Store all Sandbox *.cpp source files in a variable
for /r %srcdir% %%F in (*.cpp) do (
    call :relpath %%F
    call set "srcs=%%srcs%% %root%\!relpath!"
)

rem Compile *.cpp files
echo Compiling application code...
cl /EHsc /c /std:c++20 /JMC /MDd /Zi /W4 /Wall /Fo%objdir%\ /Fd%objdir%\ %incs% %srcs% 
if %errorlevel% neq 0 goto :error
echo Application code compiled successfully & echo.

rem Compile Sandbox program and link `*.lib` Object File Library

rem Store all *.obj file names only for the linker
for /r %objdir% %%F in (*.obj) do (
    call set "objs=%%objs%% %objdir%\%%~nxF"
)

rem Link *.obj object files
echo Generating Executable...
LINK /DEBUG %objs:~1% /SUBSYSTEM:CONSOLE /OUT:%exepath% %libfilepath% %links% 
if %errorlevel% neq 0 goto :error
call :relpath %bindir%
echo Application exectuable `%exefilename%` created in `%relpath%` successfully. & echo.

rem Build completed successfully

echo Build succeeded.
goto :eof

rem Gets the relative path from the given absolute path
:relpath
set "abspath=%1%"
set "relpath=!abspath:%root%=!"
goto :eof

rem Adds a compile flag to `compile_flags.txt` for the Clang LSP.
:addcompileflag
set flag=%1
rem Invert slashes for include paths to be compatible with the LSP
set cpath=!flag:\=/!
echo -I>> %root%\%compileflags%
echo .!cpath!>> %root%\%compileflags%
goto :eof

rem Error function to call when a build error is encountered
:error
set err=%errorlevel%
echo. 
echo Build failed.
echo Exited with error code: %errorlevel%
exit /b %errorlevel%
