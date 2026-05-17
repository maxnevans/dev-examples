# C++20 Modules example with DLL

This is an example of C++ modules project with DLL.

## 1. Set environment variables if you want to compile with a specific compiler (e.g. clang).
Hint. In powershell:
```bash
$env:CXX="clang++"
$env:CC="clang"
```

## 2. Generate and build the project
```bash
cmake -S . -B _build
cmake --build _build --target main
```

Hint. If you want clang, use Ninja generator:
> [!CAUTION]
> This is not going to compile on Windows. Read the **Current problems** section about the details of why and how. Though, should compile find on Linux.
```bash
cmake -GNinja -S . -B _build
cmake --build _build --target main
```

## Current problems
#### 1.  VS Code doesn't support C++ modules. Syntax highlighting is broken.

Track the issue: [Github Issue 6302 - Add IntelliSense for C++20 modules importing](https://github.com/microsoft/vscode-cpptools/issues/6302)

#### 2. Clang as of 17.0.6 and even 18.1.0-rc3 doesn't support STD header units on Windows

`import <vector>;` - this is going to produce compilation errors.
The only possible solution is to use **module.modulemap** files [Stackoverflow Thread](https://stackoverflow.com/questions/67210597/how-to-compile-use-header-unit-modules-under-clang-c/67254709#67254709).