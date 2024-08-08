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
if not exist obj\x64 mkdir obj\x64
if not exist obj\x86 mkdir obj\x86
if not exist bin mkdir bin
if not exist bin\x64 mkdir bin\x64
if not exist bin\x86 mkdir bin\x86

pushd obj

if %target%==x64 ( pushd x64 ) else ( pushd x86 )

call :sources src
call :sources src\win32

goto :compile

rem Reference win32 specific source files
:sources
for %%F in (..\..\%1\*.cpp) do (
    call set "srcs=%%srcs%% %1\%%~nxF"
)

rem End of :sources subroutine call
goto :eof

:compile
rem pop to obj dir and pop to root dir
popd
popd

cl /Fo"obj\\%platform%\\" -MT -GR- -EHa- -Oi -WX -W4 -wd4201 -wd4100 -wd4189 -wd4505 %compilerFlags% /c -FAsc /Fa"obj\\%target%\\" -Z7 %srcs:~1%

pushd bin

if %target%==x64 ( pushd x64 ) else ( pushd x86 )

for /r ..\..\obj %%F in (%platform%\*.obj) do (
    call set "objs=%%objs%% obj\%target%\%%~nxF"
)

rem Pop to bin dir and pop to root dir
popd
popd

LINK %linkerFlags% %objs:~1% /MAP:bin\%target%\iconoclast_%target%.map /OUT:bin\%target%\iconoclast_%target%.exe /SUBSYSTEM:WINDOWS,%subsysVer% user32.lib gdi32.lib winmm.lib

rem Build completed
