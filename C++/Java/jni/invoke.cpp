/**
Calling Java code from C++
Tested on:
    - Oracle Java JDK 8 and Ms. Visual Studio 2017 Community Ed.

All experiment is using execution schema "load .class file"
See also "schema.cpp" inside directory "schemas" for other schemas.

Include:
    - $JDK/include
    - $JDK/include/win32

Lib:
    - $JDK/lib/jvm.lib

Compile:
    (GCC)
    $ g++ invoke.cpp -o invoke -I$JDK/include -I$JDK/include/win32 -L$JDK/lib -ljvm -std=c++11

    (MSVC)
    $ cl invoke.cpp /I $JDK/include /I $JDK/include/win32 $JDK/lib/jvm.lib 

Run:
    $ invoke

Notes:
    - do not put jvm.dll on same directory
    - set PATH to JRE where jvm.dll reside, ex:
        $JDK/jre/bin/server    , if using JDK
        or 
        $JRE/bin/server        , if using JRE
    - I still cannot make G++ works
*/
#include <iostream>
#include <jni.h>

//========================== Static & Globals ==========================


//========================== Helper  Function ==========================

/*
Before using JNI in C++, load and initialize the Java Virtual Machine (JVM).
Then load the Java bytecode, which will be executed by the JVM.
*/
JNIEnv* create_vm(JavaVM** jvm);

//=====================================================================

int main()
{
    JavaVM       *jvm;                 // Pointer to the JVM (Java Virtual Machine)
    JNIEnv       *env;                 // Pointer to native interface
    jclass       cls, javclass;
    jmethodID    magic, hello, multi, ctor, showid;

    //===== Prepare loading of Java VM =====
    env = create_vm(&jvm);

    //===== Doing something with JVM =====
    try 
    {
        cls = env->FindClass("called");     // If the class is in package, then use "package/class" notation

    // Example 1: static method that return integer
        magic  = env->GetStaticMethodID(cls, "magic", "()I");
        if (magic == nullptr)
            std::cerr << "ERROR: method 'magic' is not found!" << std::endl;
        else 
        {
            jint res = env->CallStaticIntMethod(cls, magic);
            std::cout << "Result is: " << res << std::endl;
        }
        
    // Example 2: static method that require array of string
        hello  = env->GetStaticMethodID(cls, "hello", "([Ljava/lang/String;)V");
        if (hello == nullptr)
            std::cerr << "ERROR: method 'hello' is not found!" << std::endl;
        else
        { 
            jobjectArray names = env->NewObjectArray(5,     // constructs java array of 5
				env->FindClass("java/lang/String"),         // Strings
				env->NewStringUTF("Reversing.ID"));         // each initialized with value "str"
			env->SetObjectArrayElement(names, 2, env->NewStringUTF("Xathrya"));  // change an element
			env->CallStaticVoidMethod(cls, hello, names);   // call the method with the arr as argument.
			env->DeleteLocalRef(names);                     // release the object
        }

    // Example 3: static method that require multiple argument
        multi  = env->GetStaticMethodID(cls, "multi", "(II)V");
        if (multi == nullptr)
            std::cerr << "ERROR: method 'multi' is not found!" << std::endl;
        else 
            env->CallStaticVoidMethod(cls, multi, 10, 30);
    
    // Example 4: Instantiate a class by constructor
        ctor = env->GetMethodID(cls, "<init>", "()V");
        if (ctor == nullptr)
            std::cerr << "ERROR: constructor is not found!" << std::endl;
        else 
        {
            jobject called = env->NewObject(cls, ctor); 
            if (called)
            {
        
    // Example 5: method that return string
                showid = env->GetMethodID(cls, "showId", "()Ljava/lang/String;");
                if (showid == nullptr)
                    std::cerr << "ERROR: method 'multi' is not found!" << std::endl;
                else
                {
                    jstring s = (jstring) env->CallObjectMethod(called, showid);
                    const char * id = env->GetStringUTFChars(s, NULL);
                    std::cout << id <<std:: endl;
                    env->ReleaseStringUTFChars(s, id);
                }
            }
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    //===== Destroy JVM =====
    jvm->DestroyJavaVM();

    return 0;
}

JNIEnv* create_vm(JavaVM** jvm)
{
    JNIEnv* env;
    JavaVMInitArgs vm_args;                                 // Initialization arguments    
    JavaVMOption  options[2];                               // JVM invokation options
    
    jint version;

    // Set the options for JVM
    options[0].optionString = "-Djava.class.path=.";        // where to find java .class
    options[1].optionString = "-DXcheck:jni:pedantic";      // some JNI stuffs

    // Set the arguments for JVM
    vm_args.version = JNI_VERSION_1_6;                      // Minimum java version
    vm_args.nOptions = 2;                                   // Number of options
    vm_args.options = options;                         
    vm_args.ignoreUnrecognized = false;                     // Invalid options make the JVM init fail.

    //===== Load and initialize VM and JNI interface =====
    jint rc = JNI_CreateJavaVM(jvm, (void**) &env, &vm_args);
    if (rc != JNI_OK || env == nullptr) 
    {   // Error processing...
        std::cout <<  "Unable to launch JVM: " << rc << std::endl;
        exit(1);
    }
    std::cout << "JVM load successfully" << std::endl;
    version = env->GetVersion();
    std::cout << "Version: " << ((version >> 16) & 0x0F) << "." << (version & 0x0F) << std::endl;
    std::cout << std::endl;

    return env;
}