# This is required version to support C++ modules - 3.28. There is no more need in additional experimental feature enabling since 
# GCC joined the band and supported modules as well as other compilers. 
cmake_minimum_required(VERSION 3.28)
project(cpp_modules_with_dll_example CXX)

# Turning off extensions avoids an issue with the clang 16 compiler
# clang 17 and greater can avoid this setting
set(CMAKE_CXX_EXTENSIONS OFF)
# Set the version of C++ for the project
set(CMAKE_CXX_STANDARD 20)

# To support generate_export_header
include(GenerateExportHeader)

# Create a shared library
add_library(foo SHARED)

# Generate, include and add a source of export header to add __declspec(import) and __declspec(export)
generate_export_header(foo BASE_NAME foo EXPORT_MACRO_NAME foo_API EXPORT_FILE_NAME GeneratedSources/fooAPI.h)
target_include_directories(foo PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/GeneratedSources")
target_sources(foo PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/GeneratedSources/fooAPI.h")

# Add the module file to the library
target_sources(foo
  PUBLIC
    FILE_SET CXX_MODULES FILES
      foo.cpp
      foo_impl.cpp
)

# Create an executable
add_executable(main main.cpp)
# Link to the library foo
target_link_libraries(main foo)