@echo off
setlocal

gcc -O2 -Wall -Wextra -std=c11 -o tab.exe tab.c

if errorlevel 1 (
    echo Build failed.
    exit /b 1
)

echo Build succeeded: tab.exe
endlocal
