#include "foo/foo.h"
#include "bar/bar.h"
#include "gobby/gobby.h"
#include <vector>
#include <cstdint>

int main()
{
    Foo foo;
    std::vector<std::int32_t> Some;
    foo.PrintHelloWorld(10, Some);

    Bar bar;
    std::vector<std::int32_t> Another;
    bar.PrintHelloWorldFromBar(20, Another);

    Gobby gobby;
    std::vector<std::int32_t> Hello;
    gobby.PrintHelloWorldFromGobby(30, Hello);
    return 0;
}