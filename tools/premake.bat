@echo off

rem Set project root directory
set root=%~dp0..
for %%I in ("%ROOT%") do set "root=%%~fI"

call %root%\vendor\premake-core\bin\release\premake5.exe vs2026
