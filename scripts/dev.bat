@echo off

REM Call to start development mode

if not defined DevEnvDir (
    call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
)

start nvim-qt .
