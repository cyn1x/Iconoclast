@echo off

setlocal enabledelayedexpansion

set err=

set argCount=0
for %%x in (%*) do (
    set /A argCount+=1
    set "argVec[!argCount!]=%%~x"
)

set target=x64
set compilerFlags=-DDEBUG=1
set config=Debug
set linkerFlags=/DEBUG

set root=%~dp0..
for %%I in ("%ROOT%") do set "ROOT=%%~fI"

for /L %%i in (1,1,%argCount%) do (
    set arg=!argVec[%%i]!

    if !arg! EQU clean goto :clean
    if !arg! EQU release (
        set compilerFlags=-Od -DDEBUG=0
        set linkerFlags=/OPT:REF
        set config=Release
    )
    if !arg! EQU x86 (
        set target=x86
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
    call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" %target%
)

set dll=iconoclast_%target%.dll
set exe=Sandbox.exe
set flags=compile_flags.txt

set libs=user32.lib gdi32.lib dxgi.lib d3d11.lib d3dcompiler.lib opengl32.lib

rem Clear `compile_flags.txt` file
echo | set /p clear="" > %flags%

rem Make required directories for core engine and sandbox environment
call :mkdirs Iconoclast
call :mkdirs Sandbox

goto :build

:mkdirs
rem Make required directories for engine compilation
if not exist %1\obj mkdir %1\obj
if not exist %1\obj\Windows\%config%_x64 mkdir %1\obj\Windows\%config%_x64
if not exist %1\obj\Windows\%config%_x86 mkdir %1\obj\Windows\%config%_x86
if not exist %1\bin mkdir %1\bin
if not exist %1\bin\Windows\%config%_x64 mkdir %1\bin\Windows\%config%_x64
if not exist %1\bin\Windows\%config%_x86 mkdir %1\bin\Windows\%config%_x86

goto :eof

:build

rem Build Sandbox project

rem Set obj directory for the Iconoclast project
set objdir=%root%\Iconoclast\obj\Windows\%config%_%platform%\

call :include
call :sources
goto :compile

rem Set all relative include directory paths
:include
rem Reference the absolute and relative paths of the header file
for /r %root%\Iconoclast\include %%F in (*.h) do (

    set "abspath=%%~dpF"
    set "relpath=!abspath:%root%=!"

    rem Prevent duplicate include directories
    if !prev! neq !relpath! (
        call set "incs=%%incs%% -I%root%!relpath:~0,-1!"

        rem Invert slashes for include paths to be compatible with the LSP
        set cpath=!relpath:\=/!
        echo -I>> %root%\%flags%
        echo .!cpath!>> %root%\%flags%

        set "prev=!relpath!"

    )
)

rem End of :include subroutine call
goto :eof

rem Recursively set all relative file paths of *.cpp source files
:sources
for /r %root%\Iconoclast\src %%F in (*.cpp) do (

    if %%~nF neq IconoclastPCH (
        set "abspath=%%F"
        set "relpath=!abspath:%root%=!"
        call set "srcs=%%srcs%% %root%\!relpath!"
    )
)

rem End of :sources subroutine call
goto :eof

:compile
cl /EHsc /c /std:c++20 /MDd /FAsc /Zi /WX /W4 /Yc"IconoclastPCH.h" %incs% %root%\Iconoclast\src\IconoclastPCH.cpp /Fo%objdir% /Fp"%objdir%\IconoclastPCH.pch" /Fd"%objdir%\vc140.pdb" /Fa"%objdir%\IconoclastPCH.asm"
if %errorlevel% neq 0 goto :error

cl /nologo /EHsc /c /std:c++20 /JMC /MDd /FAsc /Zi /Yu"IconoclastPCH.h" /Fp"%objdir%IconoclastPCH.pch" /WX /W4 -wd4201 -wd4100 -wd4189 -wd4505 /Fo%objdir% %incs% %compilerFlags% /Fd"%objdir%\vc140.pdb" /Fa%objdir% %srcs:~1%
if %errorlevel% neq 0 goto :error
echo Engine code compiled successfully & echo.

rem Store all *.obj file names only for the linker
for /r %root%\Iconoclast\obj\Windows\%config%_%target% %%F in (*.obj) do (
    call set "objs=%%objs%% %objdir%\%%~nxF"
)

echo Creating library...
LIB /SUBSYSTEM:WINDOWS %objs:~1% /OUT:%root%\Iconoclast\bin\Windows\%config%_%target%\iconoclast_%target%.lib
if %errorlevel% neq 0 goto :error
echo Engine library file `iconoclast_%target%.lib` created successfully & echo.

rem Build Sandbox project

rem Set obj directory for the Sandbox project
set objdir=%root%\Sandbox\obj\Windows\%config%_%platform%\

rem Store all Sandbox *.cpp source files in a variable
for /r %root%\Sandbox\src %%F in (*.cpp) do (
    set "abspath=%%F"
    set "relpath=!abspath:%root%=!"
    call set "exesrcs=%%exesrcs%% %root%\!relpath!"
)

rem Compile *.cpp files
echo Compiling application code...
cl /EHsc /c /std:c++20 /JMC /MDd /Zi /W4 /Wall /Fo%objdir% %incs% /Fd%objDir% %exesrcs% 
if %errorlevel% neq 0 goto :error
echo Application code compiled successfully & echo.

rem Compile Sandbox program and link DLL

rem Store all *.obj file names only for the linker
for /r %root%\Sandbox\obj\Windows\%config%_%target% %%F in (*.obj) do (
    call set "exeobjs=%%exeobjs%% %root%\Sandbox\obj\Windows\%config%_%platform%\%%~nxF"
)

rem Link *.obj object files
echo Generating Executable...
LINK /DEBUG %exeobjs:~1% /SUBSYSTEM:CONSOLE /OUT:%root%\Sandbox\bin\Windows\%config%_%target%\%exe% %libs% %root%\Iconoclast\bin\Windows\%config%_%target%\iconoclast_%target%.lib
if %errorlevel% neq 0 goto :error
echo Application exectuable `%exe%` created successfully & echo.

rem Build completed successfully

echo Build completed
goto :eof

rem Error function to call when a build error is encountered
:error
set err=%errorlevel%
echo. 
echo Build failed
echo Exited with error code: %errorlevel%
exit /b %errorlevel%
