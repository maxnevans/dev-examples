#include "foo.h"
#include <algorithm>
#include <iostream>

void Foo::PrintHelloWorld(std::int32_t Amount, std::vector<std::int32_t>& Apples)
{
    std::ranges::sort(Apples);
    std::cout << Amount;
}