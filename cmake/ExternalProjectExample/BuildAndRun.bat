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
cmake -DCMAKE_CXX_COMPILER:STRING=clang -DCMAKE_RC_COMPILER:STRING=llvm-rc -DCMAKE_EXPORT_COMPILE_COMMANDS=TRUE -S . -B _build -G "Ninja Multi-Config" -DCMAKE_BUILD_TYPE=Release

if %ERRORLEVEL% NEQ 0 (
    echo Error: Failed to configure the project.
    exit /b 1
)

:past_reconfigure

:: Build cmake project with the generator and compiler specified on configuration step
cmake --build _build --config Release --target ExternalProjectExample

if %ERRORLEVEL% NEQ 0 (
    echo Error: Failed to build the project.
    exit /b 1
)

:past_build

:: Run the executable built
call "%~dp0_build\Release\ExternalProjectExample.exe"

if %ERRORLEVEL% NEQ 0 (
    echo Error: Failed to run benchmarks.
    exit /b 1
)