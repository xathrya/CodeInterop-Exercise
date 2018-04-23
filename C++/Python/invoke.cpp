/**
Calling Python code from C++
Tested on:
    - Python 3.6.4

Include:
    - $PYTHON/include

Lib:
    - $PYTHON/libs/

Compile:
    (GCC)
    $ g++ invoke.cpp -o invoke -I$PYTHON/include -L$PYTHON/libs -lpython36 -std=c++11

    (MSVC)
    $ cl invoke.cpp /I $PYTHON/include $PYTHON/libs/python36.lib 

Run:
    $ invoke
*/
#include <iostream>
#include <Python.h>


void create_vm();

int main(int argc, char* argv[])
{
    using namespace std;

    wchar_t * program = Py_DecodeLocale(argv[0], NULL);
    if (program == nullptr)
    {
        cerr << "Fatal error: cannot decode " << argv[0] << endl;
        exit(1);
    }
    Py_SetProgramName(program);     // Optional but recommended
    Py_Initialize();
    PyRun_SimpleString("from time import time, ctime; print('Today is %s' % ctime(time()))");

    if (Py_FinalizeEx() < 0)
        exit(120);
    
    PyMem_RawFree(program);
    return 0;
}

void create_vm()
{

}