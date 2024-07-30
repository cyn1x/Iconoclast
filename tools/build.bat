@echo off

setlocal enabledelayedexpansion

set argCount=0
for %%x in (%*) do (
    set /A argCount+=1
    set "argVec[!argCount!]=%%~x"
)

set target=x64
set subsysVer=6.02
set compilerFlags=-DDEBUG=1
set linkerFlags=/DEBUG

for /L %%i in (1,1,%argCount%) do (
    set arg=!argVec[%%i]!

    if !arg! EQU clean goto :clean
    if !arg! EQU release (
        set compilerFlags=-Od -DDEBUG=0
        set linkerFlags=/OPT:REF
    )
    if !arg! EQU x86 (
        set target=x86
        set subsysVer=5.01
    )
)

rem End of entry point procedure
goto :main

:clean
del /S /Q obj\*.* > nul
del /S /Q bin\*.* > nul

rem End of :clean subroutine call
goto :eof

:main

if not defined DevEnvDir (
    call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" %target%
)

rem Change directory to the project root
popd

if not exist obj mkdir obj
if not exist bin mkdir bin

pushd obj

call :sources ..\src
call :sources ..\src\win32

goto :compile

rem Reference win32 specific source files
:sources
for %%F in (%1\*.cpp) do (
    call set "srcs=%%srcs%% %1\%%~nxF"
)

rem End of :sources subroutine call
goto :eof

:compile
cl -MT -GR- -EHa- -Oi -WX -W4 -wd4201 -wd4100 -wd4189 -wd4505 %compilerFlags% /c -FAsc -Z7 %srcs:~1%

popd
pushd bin

for /r ..\obj %%F in (*.obj) do (
    call set "objs=%%objs%% ..\obj\%%~nxF"
)

LINK %linkerFlags% %objs:~1% /MAP:iconoclast.map /OUT:iconoclast.exe /SUBSYSTEM:WINDOWS,%subsysVer% user32.lib gdi32.lib

popd
