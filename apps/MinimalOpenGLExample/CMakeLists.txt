cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

project(AppTemplateOpenGL CXX)

set(CMAKE_CXX_STANDARD 23)

# Enable C++20 modules for CMake 3.27 exactly - won't work with that API key for other versions
set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "aa1f7df0-828a-4fcd-9afc-2dc80491aca7")
set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP true)

set_property(GLOBAL PROPERTY USE_FOLDERS true)

include(FetchContent)

FetchContent_Declare(
    glfw
    URL "https://github.com/glfw/glfw/archive/refs/tags/3.3.8.zip"
)

FetchContent_Declare(
    glew
    URL "https://github.com/nigels-com/glew/releases/download/glew-2.2.0/glew-2.2.0.zip"
    SOURCE_SUBDIR "build/cmake"
)

FetchContent_MakeAvailable(glfw glew)

# Patch the `glew` targets - this is required because the original GLEW project requires you to build it with CMake
# and then install before the build of the dependencies (we don't install the project as FetchContent_MakeAvailable
# just add_subdirectory() with the CMakeLists.txt file of the GLFW project).
foreach(t glew glew_s)
  target_include_directories(${t} PUBLIC $<BUILD_INTERFACE:${glew_SOURCE_DIR}/include>)
endforeach()

find_package(OpenGL)

add_executable(AppTemplateOpenGL)
set_directory_properties(PROPERTIES VS_STARTUP_PROJECT AppTemplateOpenGL)

target_sources(AppTemplateOpenGL
  PRIVATE
    FILE_SET cxx_modules TYPE CXX_MODULES
      FILES
        Source/Main.cpp
        Source/OpenGL.mpp
)

target_link_libraries(AppTemplateOpenGL PRIVATE OpenGL::GL glfw glew_s)