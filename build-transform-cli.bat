@echo off
setlocal

set "ROOT_DIR=%~dp0"
set "GPP=D:\mingw64\bin\g++.exe"

"%GPP%" -std=c++17 ^
  tools/transform_cli.cpp ^
  core/transform/source.cpp ^
  core/transform/hellas.cpp ^
  core/transform/jugoslav.cpp ^
  core/transform/choseon.cpp ^
  -o transform_cli.exe
if errorlevel 1 goto :fail

echo Build complete: transform_cli.exe
exit /b 0

:fail
echo Build failed.
exit /b 1
