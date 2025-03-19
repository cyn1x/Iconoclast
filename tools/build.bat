@echo off

setlocal enabledelayedexpansion

set argCount=0
for %%x in (%*) do (
    set /A argCount+=1
    set "argVec[!argCount!]=%%~x"
)

set target=x64
set compilerFlags=-DDEBUG=1
set config=Debug
set linkerFlags=/DEBUG

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

set dll=iconoclast_%target%.dll
set exe=Sandbox.exe
set flags=compile_flags.txt

rem/||(
Store the absolute ^path of the project root directory in a variable 
followed by a slash to stay consistent with `~dp0` when using 
functions ^for splitting strings and ^set the comparator variable. ^)
)
set root=%cd%\
set cmp=!root:~0,-1!

if not defined DevEnvDir (
    call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" %target%
)

echo | set /p clear="" > %flags%

rem Change directory to the project root
popd

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

rem Change dir to Iconoclast project dir
pushd Iconoclast

call :include

rem Change directory to intermediate dir
pushd obj
pushd Windows

if %target%==x64 ( pushd %config%_x64 ) else ( pushd %config%_x86 )

call :sources
goto :compile

rem Set all relative include directory paths
:include
rem Reference the absolute and relative paths of the header file
for /r include %%F in (*.h) do (

    set "abspath=%%~dpF"
    set "relpath=!abspath:%cmp%=!"

    rem Prevent duplicate include directories
    if !prev! neq !relpath! (
        call set "incs=%%incs%% -I..\..\..\..!relpath:~0,-1!"

        rem Invert slashes for include paths to be compatible with the LSP
        set cpath=!relpath:\=/!
        echo -I>> ..\%flags%
        echo .!cpath!>> ..\%flags%

        set "prev=!relpath!"
    )
)

rem End of :include subroutine call
goto :eof

rem Recursively set all relative file paths of *.cpp source files
:sources
for /r ..\..\..\src %%F in (*.cpp) do (
    set "abspath=%%F"
    set "relpath=!abspath:%cmp%=!"
    call set "srcs=%%srcs%% ..\..\..\..!relpath!"
)

rem End of :sources subroutine call
goto :eof

:compile
cl /EHsc /c /LD -MD -Z7 /Yu"precompiled.h" /Yc"precompiled.h" -GR- -EHa- -Oi -WX -W4 -wd4201 -wd4100 -wd4189 -wd4505 -FAsc /std:c++20 /Fo"..\..\..\obj\\Windows\\%config%_%platform%\\" %incs% %compilerFlags% /Fa"..\..\..\obj\\Windows\\%config%_%target%\\" %srcs:~1%

rem Pop to platform dir, bin dir, and project dir
popd
popd
popd

pushd bin
pushd Windows

if %target%==x64 ( pushd %config%_x64 ) else ( pushd %config%_x86 )

rem Store all *.obj file names only for the linker
for /r ..\..\..\obj %%F in (%config%_%platform%\*.obj) do (
    call set "objs=%%objs%% obj\Windows\%config%_%target%\%%~nxF"
)

rem Pop to platform dir, bin dir, and root dir
popd
popd
popd

LINK /nologo %linkerFlags% %objs:~1% /MAP:bin\Windows\%config%_%target%\iconoclast_%target%.map /DLL /OUT:bin\Windows\%config%_%target%\iconoclast_%target%.dll user32.lib gdi32.lib

popd

rem Copy DLL to where the test executable will be built
copy Iconoclast\bin\Windows\%config%_%target%\%dll% Sandbox\bin\Windows\%config%_%target%\%dll% > nul

rem Change directory to Sandbox project dir and intermediate dir
pushd Sandbox
pushd obj
pushd Windows

if %target%==x64 ( pushd %config%_x64 ) else ( pushd %config%_x86 )

set cmp=!root:~0!Sandbox

rem Store all Sandbox *.cpp source files in a variable
for /r ..\..\..\src %%F in (*.cpp) do (
    set "abspath=%%F"
    set "relpath=!abspath:%cmp%=!"
    call set "exesrcs=%%exesrcs%% ..\..\..!relpath!"
)

rem Compile *.cpp files
cl /nologo /EHsc /c /MD /Zi /W4 /Wall /std:c++20 /Fo"..\..\..\obj\\Windows\\%config%_%platform%\\" /Fd"..\..\..\obj\\Windows\\%config%_%platform%\\" %exesrcs% %incs%

rem Compile Sandbox program and link DLL

rem Pop to obj dir and then project dir
popd
popd
popd

rem Store all *.obj file names only for the linker
for /r obj %%F in (*.obj) do (
    call set "exeobjs=%%exeobjs%% obj\Windows\%config%_%target%\%%~nxF"
)

rem Link *.obj object files
LINK /DEBUG %exeobjs:~1% /OUT:bin\Windows\%config%_%target%\%exe% ..\Iconoclast\bin\Windows\%config%_%target%\iconoclast_%target%.lib

rem Build completed
