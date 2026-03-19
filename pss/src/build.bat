@echo off
setlocal

gcc -O2 -Wall -Wextra -std=c11 -o pss.exe pss.c

if errorlevel 1 (
    echo Build failed.
    exit /b 1
)

echo Build succeeded: pss.exe
endlocal
