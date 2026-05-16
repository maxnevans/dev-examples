cmake_minimum_required(VERSION 3.29.0-rc3 FATAL_ERROR)
project(ExternalProjectExample LANGUAGES CXX)

set(benchmark_LIB ${CMAKE_BINARY_DIR}/GBenchmark/Install/lib/benchmark.lib)
set(benchmark_DLL ${CMAKE_BINARY_DIR}/GBenchmark/Install/bin/benchmark.dll)

include(ExternalProject)
# Configures, build and install at BUILD time of the main project -> target is not available at configuration time of the main project
# -> can't link against it directly.
ExternalProject_Add(
    GBenchmark
    URL "https://github.com/google/benchmark/archive/refs/tags/v1.8.3.zip"

    # 
    PREFIX ${CMAKE_BINARY_DIR}/GBenchmark

    # This is because of the bug in google/benchmark that treats a warning like an error even though BENCHMARK_ENABLE_WERROR=OFF
    PATCH_COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/patch/benchmark.cc ${CMAKE_BINARY_DIR}/GBenchmark/src/GBenchmark/src/benchmark.cc

    # Nothing to update here, so disable this command entirely
    UPDATE_COMMAND ""

    # Ninja generator requires explicit byproducts
    BUILD_BYPRODUCTS
        ${benchmark_LIB}
        ${benchmark_DLL}

    CMAKE_ARGS
        # External project installs it to what main project ${CMAKE_INSTALL_PREFIX} specifies (which is something like C:\Program Files (x86)\ExternalProjectExample
        # So change this behavior to local one.
        -D CMAKE_INSTALL_PREFIX:PATH=${CMAKE_BINARY_DIR}/GBenchmark/Install

        # Have to build shared libs for this version of google/benchmark as it doesn't work with static libs when ninja+clang used on windows for whatever reason
        -D BUILD_SHARED_LIBS:BOOL=TRUE

        # This demonstrates the power of multiconfig builds - even if using multiconfig and real configuration is decided at build time of CMake, we can still build
        # dependencies based on the build time configuration (see BuildAndRun.bat cmake --build command, --config $<CONFIG>).
        # Also not that we changed BUILD_COMMAND of ExternalProject
        -D CMAKE_BUILD_TYPE:STRING=$<CONFIG>

        -D BENCHMARK_ENABLE_GTEST_TESTS=OFF
        -D BENCHMARK_ENABLE_TESTING=OFF

        # We don't care about compilation warnings as we're not the developers of google/benchmark to fix them.
        -D BENCHMARK_ENABLE_WERROR=OFF

        # We don't care about cmake warnings of google/benchmark project itself as we're not the developers of it to fix them.
        -Wno-dev

    # We have to change the build command because we wanna be in control of the configuration in this case (here we just make it the same as what supplied to the main project).
    # From interesting here notice that we don't wanna change output binary dir but use the one ExternalProject provides itself for us, so we use <BINARY_DIR> placeholder
    # that works specifically inside ExternalProject command to let it substitute for the real path.
    BUILD_COMMAND ${CMAKE_COMMAND} --build <BINARY_DIR> --config $<CONFIG>
)

# We have to precreate Include directory, otherwise INTERFACE_INCLUDE_DIRECTORIES will give us an error of non-existing directory
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/GBenchmark/Install/Include)

# As ExternalProject doesn't provide a target at configuration time of the main project (ExternalProjectExample), we have to create a mock target
# and fill all the data ourselves. This can be literally the content of GBenchmark/Install/lib/cmake/benchmark/benchmarkConfig.cmake file.
# On the non-first build we'll have this benchmarkConfig.cmake available and can use it instead of our own.
# This is a bit risky to make things like that because our configuration may differ from the original developers' one. Also it's going to be
# a problem if we change configuration of the benchmark for example from Release to Debug after we compiled it at least once as 
# configuration stage of the main project is going to use Release config of benchmark but the benchmark itself is going to be recompiled into Debug
# just on the next stage. So using find_package like this is a bit risky if dependency configuration changes.

#find_package(benchmark PATHS ${CMAKE_BINARY_DIR}/GBenchmark/Install)
if(NOT benchmark_FOUND)
    message(STATUS "benchmark dependency not found, creating mock for first compilation time.")

    # Notice that if you changed BUILD_SHARED_LIBS in ExternalProject you have to change it here as well from SHARED to STATIC
    # and change IMPORTED_IMPLIB & IMPORTED_LOCATION properties (I believe on Windows IMPORTED_LOCATION should start point to .lib instead of .dll
    # and IMPORTED_IMPLIB should be just empty and is ignored in that case).
    add_library(benchmark::benchmark SHARED IMPORTED)
    set_target_properties(benchmark::benchmark PROPERTIES
        IMPORTED_IMPLIB
        ${benchmark_LIB}
        IMPORTED_LOCATION
        ${benchmark_DLL}
        INTERFACE_INCLUDE_DIRECTORIES
        ${CMAKE_BINARY_DIR}/GBenchmark/Install/Include
    )
    # Don't forget to add a dependency on the ExternalProject target itself
    add_dependencies(benchmark::benchmark GBenchmark)
else()
    message(STATUS "benchmark dependency successfully found, using provided by developers benchmarkConfig.cmake")
endif()


add_executable(ExternalProjectExample main.cpp)
target_link_libraries(ExternalProjectExample PRIVATE benchmark::benchmark)

# 
function(copy_dlls target)
    if(WIN32)
        add_custom_command(
            TARGET "${target}" POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy -t "$<TARGET_FILE_DIR:${target}>"
            "$<TARGET_RUNTIME_DLLS:${target}>" USES_TERMINAL COMMAND_EXPAND_LISTS
        )
    endif()
endfunction()

# Have to copy dlls of the libraries our ExternalProjectExample depends on to the location where ExternalProjectExample.exe is to be able start it: benchmark::benchmark
copy_dlls(ExternalProjectExample)