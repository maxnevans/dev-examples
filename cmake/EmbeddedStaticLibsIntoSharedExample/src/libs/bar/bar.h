#include "barAPI.h"

#include <cstdint>
#include <vector>

class bar_API Bar
{
public:
    void PrintHelloWorldFromBar(std::int32_t Amount, std::vector<std::int32_t>& Apples);
};