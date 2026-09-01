@echo off
setlocal

set "LAUNCHER_DIR=%~dp0"
set "ROOT_DIR=%LAUNCHER_DIR%.."
set "GPP=D:\mingw64\bin\g++.exe"

pushd "%ROOT_DIR%"
"%GPP%" -std=c++17 ^
  tools/transform_cli.cpp ^
  core/transform/source.cpp ^
  core/transform/hellas.cpp ^
  core/transform/jugoslav.cpp ^
  core/transform/choseon.cpp ^
  core/transform/biela_old.cpp ^
  core/transform/biela_new.cpp ^
  core/transform/ukrajina.cpp ^
  core/transform/latin_old.cpp ^
  core/transform/rusnew_old.cpp ^
  core/transform/rusold_old.cpp ^
  core/transform/forsitojik.cpp ^
  core/transform/hayeren_big.cpp ^
  core/transform/kartuli.cpp ^
  -o transform_cli.exe
if errorlevel 1 goto :fail

echo ************************************************************
echo *                                                          *
echo *           Build complete: transform_cli.exe              *
echo *                                                          *
echo ************************************************************
popd
exit /b 0

:fail
popd
echo ************************************************************
echo *                                                          *
echo *                Build failed!                             *
echo *                                                          *
echo ************************************************************
exit /b 1
