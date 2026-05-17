module foo;

import <iostream>;
import <algorithm>;

void Foo::PrintHelloWorld(std::int32_t Amount, std::vector<std::int32_t>& Apples)
{
    std::ranges::sort(Apples);
    std::cout << Amount;
}