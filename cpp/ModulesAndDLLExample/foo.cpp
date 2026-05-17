module;

#include "fooAPI.h"

export module foo;

export import <cstdint>;
export import <vector>;

export class foo_API Foo
{
public:
    void PrintHelloWorld(std::int32_t Amount, std::vector<std::int32_t>& Apples);
};