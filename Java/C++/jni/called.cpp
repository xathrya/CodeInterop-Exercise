/**
Create JNI Header:
    $ javah -jni invoke

Include:
    - $JDK/include
    - $JDK/include/win32

Compile:
    (GCC)
    $ g++ -shared called.cpp -o called.dll -I$JDK/include -I$JDK/include/win32
    (MSVC)
    $ cl invoke.cpp /I $JDK/include /I $JDK/include/win32 
*/
#include <jni.h>
#include <iostream>
#include "invoke.h"

JNIEXPORT void JNICALL 
Java_invoke_print_1jni (JNIEnv * env, jobject ptr)
{
    std::cout << "Calling C++ from Java!" << std::endl;
    env->CallVoidMethod(ptr, env->GetMethodID(env->FindClass("invoke"), "print_java", "()V"));
    return;
}