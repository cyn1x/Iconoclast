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

rem Default build configuration
set config=Debug

rem Set variable to hold various MSBuild options
set "opts= "

rem Set project root directory
set root=%~dp0..
for %%I in ("%ROOT%") do set "root=%%~fI"

rem Process command-line arguments arguments
for /L %%i in (1,1,%argCount%) do (
    
    set arg=!argVec[%%i]!

    if !arg! EQU clean goto :clean

    if !arg! EQU release (
        set config=Release
    )
    if !arg! EQU rebuild (
        call set "opts=%%opts%% -t:Rebuild"
    )
    
)

rem End of entry point procedure
goto :main

rem ==========================
rem Clean
rem ==========================
:clean

call MSBuild Iconoclast.sln -t:clean

rem End of :clean subroutine call
goto :eof

if not defined DevEnvDir (
    call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" %platform%
)

rem ==========================
rem Main
rem ==========================
:main

rem Define `compile_flags.txt` file for the LLVM Clang LSP
set compileflags=compile_flags.txt

rem Clear `compile_flags.txt` file
echo | set /p clear="" > %compileflags%

rem Set the `include` directory
set incdir=%root%\Iconoclast\include

rem Clear variables to store include filenames
set incs=

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

rem ==========================
rem Build
rem ==========================
call MSBuild Iconoclast.sln -p:Configuration=%config% %opts:~1%

rem End of `:include` subroutine call
goto :eof

rem ==========================
rem Relative Path
rem ==========================
:relpath
rem Gets the relative path from the given absolute path
set "abspath=%1%"
set "relpath=!abspath:%root%=!"
goto :eof

rem ==========================
rem Compile Flags
rem ==========================
:addcompileflag
rem Adds a compile flag to `compile_flags.txt` for the Clang LSP.
set flag=%1
rem Invert slashes for include paths to be compatible with the LSP
set cpath=!flag:\=/!
echo -I>> %root%\%compileflags%
echo .!cpath!>> %root%\%compileflags%
goto :eof

rem ==========================
rem Error
rem ==========================
:error
rem Error function to call when a build error is encountered
set err=%errorlevel%
echo. 
echo Exited with error code: %errorlevel%
exit /b %errorlevel%
