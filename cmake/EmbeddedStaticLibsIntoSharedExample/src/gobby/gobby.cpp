#include "gobby.h"
#include <algorithm>
#include <iostream>
#include "../foo/foo.h"

void Gobby::PrintHelloWorldFromGobby(std::int32_t Amount, std::vector<std::int32_t>& Apples)
{
    std::ranges::sort(Apples);
    std::cout << Amount;

    Foo foo;
    std::vector<std::int32_t> Some;
    foo.PrintHelloWorld(50, Some);
}