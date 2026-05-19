# Embed static libraries into DLL - Object libraries

```bash
cmake -G "Ninja Multi-Config" -DCMAKE_CXX_COMPILER=clang++ . -B _build
cmake --build _build --config Release --target main
./_build/Release/main.exe
```