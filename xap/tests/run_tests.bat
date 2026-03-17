@echo off
setlocal

set EXE=..\src\xap.exe

if not exist %EXE% (
    echo ERROR: %EXE% not found.
    echo Please build the program first using src\build.bat
    exit /b 1
)

for %%f in (sample_inputs\*.in) do (
    %EXE% < "%%f" > tmp.out
    fc /W tmp.out "sample_outputs\%%~nf.out" > nul

    if errorlevel 1 (
        echo FAIL: %%~nf
    ) else (
        echo PASS: %%~nf
    )
)

del tmp.out 2>nul
endlocal
