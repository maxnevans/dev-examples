@echo off

if /I "%~1"=="--no-clean" (
    goto :past_clean
)

if /I "%~1"=="--build-and-run" (
    goto :past_reconfigure
)

if /I "%~1"=="--just-run" (
    goto :past_build
)

:: Remove dir recursive (/s), don't ask approval (/q)
rd /s /q _build

:past_clean

:: Configure cmake project
:: Generate project for Visual Studio 2022, use x64 platform
cmake -G "Visual Studio 17 2022" -A x64 -Wno-dev -B "./_build" -S "./"

if %ERRORLEVEL% NEQ 0 (
    echo Error: Failed to configure the project.
    exit /b 1
)

echo:
echo ======================================
echo -- Project generated successfully!  -- 
echo ======================================
echo:

:past_reconfigure

:: Build cmake project with the generator and compiler specified on configuration step
cmake --build _build --config Release --target AppTemplateOpenGL

if %ERRORLEVEL% NEQ 0 (
    echo Error: Failed to build the project.
    exit /b 1
)

:past_build

:: Run the executable built
call "%~dp0_build\Release\AppTemplateOpenGL.exe"

if %ERRORLEVEL% NEQ 0 (
    echo Error: Failed to run project.
    exit /b 1
)