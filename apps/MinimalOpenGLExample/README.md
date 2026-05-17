# Minimal setup for OpenGL app with GLFW and GLEW on C++23 & C++20 modules

## Supported features

This template is configured to use:

- C++23 with modules
- CMake
- OpenGL (based on GLFW and GLEW)

## Supported platforms

- Windows 10 (Visual Studio 17 2022)
- (not tested) Linux
- (not tested) MacOS

## Getting up and Running

This project contains ThirdParty dependencies. After you've cloned the repo you don't have to prepare anything. The build requirements for this projects are:

- cmake 3.27 (there is a strong dependencies on the exact version, be careful - look CMakeLists.txt at the root of the project for more information)
- Visual Studio 2022 (or some other similar build system with compiler supported by cmake)

All the dependencies are fetched from github.com automatically by cmake on the configuration stage.

A simple way of Generate, Build and then Run the app on Windows 10 is to run `GenerateBuildAndRun.bat` file in the root of the repo. On the other platforms
you are __free__ to use the `cmake` commands directly (you can take a look into the `GenerateProjectFiles.bat` and `Build.bat` files for the example of 
possible build configuration).

### Development process

After the initial project generation you can use `Build.bat` or `BuildAndRun.bat` for development purposes.


## Dependencies

- GLFW (sources are compiled with the project for portability)
- GLEW (sources are compiled with the project for portability)
- OpenGL (binary lib)
