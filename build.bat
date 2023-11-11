@echo off

call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
if not exist etc mkdir etc
if not exist obj mkdir obj
if not exist bin mkdir bin
pushd obj
cl /c -Zi ..\src\win32_rakija.cpp
popd
pushd bin
LINK /DEBUG ..\obj\win32_rakija.obj /OUT:rakija.exe user32.lib gdi32.lib
popd
