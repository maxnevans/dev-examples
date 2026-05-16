# ExternalProjectExample
This is an example of how the one could use ExternalProject to add vendor library to his main project and build against it.

## Requirements
This example requires (at least it's tested to work on these, may work on other configurations as well):
- Platform: Windows 11
- Microsoft Builds Tools
    - MSVC in particular
- llvm
    - clang++
    - lld-link
    - lld-rc
- cmake
- ninja