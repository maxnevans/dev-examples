#include "bar.h"
#include <algorithm>
#include <iostream>

void Bar::PrintHelloWorldFromBar(std::int32_t Amount, std::vector<std::int32_t>& Apples)
{
    std::ranges::sort(Apples);
    std::cout << Amount;
}