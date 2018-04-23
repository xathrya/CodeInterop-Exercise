/**
Create a DLL 

Compile:
    (GCC)
    $ g++ -shared called.cpp -o called.dll
    (MSVC)
    $ cl invoke.cpp /LD /Ox /GS /sdl
*/
#include <cstdio>
using namespace std;

#ifdef __cplusplus
extern "C" {
#endif 

void print_clr(const char* str)
{
    printf("Call C++: %s\n", str);
}

#ifdef __cplusplus
}
#endif 