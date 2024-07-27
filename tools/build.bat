@echo off

setlocal enabledelayedexpansion
set arg=%1

if not defined DevEnvDir (
    call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
)

rem Change directory to the project root
popd

if not exist obj mkdir obj
if not exist bin mkdir bin

if not defined arg goto :main

if %arg%==clean (
    del /S /Q obj\*.* > nul
    del /S /Q bin\*.* > nul

    goto :eof
)

:main

pushd obj

rem Reference platform agnostic source files
for %%F in (..\src\*.cpp) do (
    call set "srcs=%%srcs%% ..\src\%%~nxF"
)

rem Reference win32 specific source files
for /r ..\src\win32 %%F in (*.cpp) do (
    call set "srcs=%%srcs%% ..\src\win32\%%~nxF"
)

cl -W4 -DDEBUG=1 /c -Zi %srcs:~1% %incs%

popd
pushd bin

for /r ..\obj %%F in (*.obj) do (
    call set "objs=%%objs%% ..\obj\%%~nxF"
)

LINK /DEBUG %objs:~1% /OUT:iconoclast.exe user32.lib gdi32.lib

popd
