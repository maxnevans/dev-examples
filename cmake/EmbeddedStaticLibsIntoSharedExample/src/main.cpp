#include "bar.h"
#include "gobby.h"
//#include "foo.h" can't include foo.h because gobby links foo privately
#include <vector>
#include <cstdint>

int main()
{
    Bar bar;
    std::vector<std::int32_t> Another;
    bar.PrintHelloWorldFromBar(20, Another);

    Gobby gobby;
    std::vector<std::int32_t> Hello;
    gobby.PrintHelloWorldFromGobby(30, Hello);
    return 0;
}