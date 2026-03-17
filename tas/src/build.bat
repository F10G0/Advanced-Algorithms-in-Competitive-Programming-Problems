@echo off
setlocal

gcc -O2 -Wall -Wextra -std=c11 -o tas.exe tas.c

if errorlevel 1 (
    echo Build failed.
    exit /b 1
)

echo Build succeeded: tas.exe
endlocal
