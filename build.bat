@echo off
setlocal enabledelayedexpansion

set "ROOT_DIR=%~dp0"
set "TMP_DIR=%ROOT_DIR%tmp"
set "UI_DIR=%ROOT_DIR%ui"
set "GPP=D:\mingw64\bin\g++.exe"
set "WINDRES=D:\mingw64\bin\windres.exe"

if not exist "%TMP_DIR%" mkdir "%TMP_DIR%"

pushd "%UI_DIR%"
"%WINDRES%" resources.rc -O coff -o "..\tmp\resources.o"
if errorlevel 1 goto :fail
popd

"%GPP%" -std=c++17 ^
  ui/main.cpp ^
  ui/js_runtime.cpp ^
  core/transform/source.cpp ^
  core/transform/hellas.cpp ^
  core/transform/jugoslav.cpp ^
  tmp/resources.o ^
  -o Lacinka.exe ^
  -municode -mwindows ^
  -loleaut32 -lole32 -luuid ^
  -lcomctl32 -lshell32 -lcomdlg32
if errorlevel 1 goto :fail

echo Build complete: Lacinka.exe
exit /b 0

:fail
echo Build failed.
exit /b 1
