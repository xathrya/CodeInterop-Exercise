/**
Create a DLL 

Compile:
    (GCC)
    $ g++ -shared called.cpp -o called.dll
    (MSVC)
    $ cl called.cpp /LD /Ox /GS /sdl
*/
#include <cstdio>
using namespace std;

#ifdef __cplusplus
extern "C" {
#endif 

//======== Type Definitions =========================================
// The callback that will be call by our functions.
typedef void (*callback)(wchar_t* str);

// Function that will be called by managed code
void __declspec(dllexport) 
print_clr(const char* str)
{
    printf("Call C++: %s\n", str);
}

void __declspec(dllexport) 
dispatch(wchar_t* input, int count, callback func)
{
    int i;
    // Call the managed code indirectly by calling the delegate function
    for (i = 0; i < count; i++)
        func(input);
}

#ifdef __cplusplus
}
#endif 