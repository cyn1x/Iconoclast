@echo off

setlocal enabledelayedexpansion
set arg=%1

if not defined DevEnvDir (
    call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
)

rem Change directory to the project root
popd

if not exist etc mkdir etc
if not exist obj mkdir obj
if not exist bin mkdir bin

if not defined arg goto :main

if %arg%==clean (
    del /S /Q obj\*.* > nul
    del /S /Q bin\*.* > nul

    goto :eof
)

:main

set flags=compile_flags.txt

rem/||(
Store the absolute ^path of the project root directory in a variable 
followed by a slash to stay consistent with `~dp0` when using 
functions ^for splitting strings and ^set the comparator variable. ^)
)
set root=%cd%\
set cmp=!root:~0,-1!

echo | set /p clear="" > %flags%

rem Iterate through all libraries and search their include dirs
for /d %%D in (lib\*) do (
    rem Append all library files for linking to the executable
    call set "libs=%%libs%% ..\%%~D\bin\%%~nD.lib"

    call :include %%~nD
    
    rem Copy library DLL to where the executable will be built
    copy lib\%%~nD\bin\*.dll bin\*.dll > nul
)

goto :compile

:include
rem Recursively set all relative include directory paths for the passed library
for /r lib\%1\include %%F in (*.h) do (

    rem Reference the absolute and relative paths of the header file
    set "abspath=%%~dpF"
    set "relpath=!abspath:%cmp%=!"

    rem Prevent duplicate include directories
    if !prev! neq !relpath! (
        call set "incs=%%incs%% -I..!relpath:~0,-1!"

        rem Invert slashes for include paths to be compatible with the LSP
        set incpath=!relpath:\=/!
        echo -I>> %flags%
        echo .!incpath!>> %flags%

        set "prev=!relpath!"
    )

)

goto :eof

:compile

pushd obj

for /r ..\src %%F in (*.cpp) do (
    call set "srcs=%%srcs%% ..\src\%%~nxF"
)

cl /c -Zi %srcs:~1% %incs%

popd
pushd bin

for /r ..\obj %%F in (*.obj) do (
    call set "objs=%%objs%% ..\obj\%%~nxF"
)

LINK /DEBUG %objs:~1% /OUT:iconoclast.exe user32.lib gdi32.lib %libs%

popd
