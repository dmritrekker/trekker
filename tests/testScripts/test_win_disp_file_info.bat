@echo off

@REM --- Function to display file information ---

set "file=%~1"

if not exist "%file%" (
    echo.
    echo %file% does not exist.
    echo.
    exit /b 1
)

for %%a in ("%file%") do (
    echo.
    echo %file% size: %%~za bytes
)

call "%~dp0trekker_win.exe" info "%file%"

@REM Check for errors from trekker_win.exe
if %ERRORLEVEL% NEQ 0 (
    echo.
    echo Error: trekker_win.exe info command failed.
    echo.
    exit /b %ERRORLEVEL%
)

echo.
