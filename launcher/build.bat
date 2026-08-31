@echo off
setlocal

set "LAUNCHER_DIR=%~dp0"
set "ROOT_DIR=%LAUNCHER_DIR%.."

pushd "%ROOT_DIR%"

call "%LAUNCHER_DIR%build-transform-cli.bat"
if errorlevel 1 goto :fail

set "CSC_IDENTITY_AUTO_DISCOVERY=false"
call npm run dist
if errorlevel 1 goto :fail

if exist "dist\Lacinka.exe" (
  copy /Y "dist\Lacinka.exe" "Lacinka.exe" >nul
) else (
  echo Build failed: dist\Lacinka.exe was not found.
  goto :fail
)

echo ************************************************************
echo *                                                          *
echo *           Build complete: %ROOT_DIR%\Lacinka.exe         *
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
