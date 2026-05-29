#include "fooAPI.h"

#include <cstdint>
#include <vector>

class foo_API Foo
{
public:
    void PrintHelloWorld(std::int32_t Amount, std::vector<std::int32_t>& Apples);
};