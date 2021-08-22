@echo off

call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
if not exist etc mkdir etc
if not exist obj mkdir obj
if not exist bin mkdir bin
pushd obj
cl /c -Zi ..\src\main.c
popd
pushd bin
LINK /DEBUG ..\obj\main.obj /OUT:rakija.exe user32.lib
popd
