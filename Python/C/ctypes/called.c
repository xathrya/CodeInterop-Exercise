/**
Calling C code from Python using CTypes

Create a library (dll / so)

Compile:
    (GCC) Windows
    $ gcc -shared called.c -o called.dll
    (GCC) Linux
    $ gcc -shared called.c -o called.so

    (MSVC)
    $ cl /nologo /LD /Ox /GS /sdl hello.c
*/
#include <stdio.h>

#ifdef _MSC_VER
#pragma comment(lib,"user32")
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32) || defined(WIN32)
#define API_TYPE __declspec(dllexport)
#else 
#define API_TYPE
#endif 

struct Vector
{
    int x;
    int y;
    int z;
};
typedef struct Vector (*VectorCallback)(struct Vector a, struct Vector b, struct Vector *c);


void API_TYPE
iop_print(const char *msg)
{
    printf("%s", msg);
}

int API_TYPE  
iop_add(int a, int b)
{
    return a + b;
}

struct Vector API_TYPE
add_vector(struct Vector a, struct Vector b)
{
    struct Vector v;
    v.x = a.x + b.x;
    v.y = a.y + b.y;
    v.z = a.z + b.z;
    return v;
}

#ifdef __cplusplus
}
#endif