/**
Calling Java code from C++
Tested on:
    - Oracle Java JDK 8 and Ms. Visual Studio 2017 Community Ed.

Schemas:
    - Execute .class file
    - Execute .class from JAR (/WAR/EAR)
    - Execute stored bytecode

Include:
    - $JDK/include
    - $JDK/include/win32

Lib:
    - $JDK/lib/jvm.lib

Compile:
    (GCC)
    $ g++ schema.cpp -o technschemaique -I$JDK/include -I$JDK/include/win32 -L$JDK/lib -ljvm -std=c++11

    (MSVC)
    $ cl schema.cpp /I $JDK/include /I $JDK/include/win32 $JDK/lib/jvm.lib 

Run:
    $ schema

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
#include <jvmti.h>
#include <cstdlib>
#include <ctime>

#include <fstream>

//========================== Static & Globals ==========================
JavaVM *jvm;                 // Pointer to the JVM (Java Virtual Machine)
JNIEnv *env;                 // Pointer to native interface

//========================== Helper  Function ==========================

/*
Before using JNI in C++, load and initialize the Java Virtual Machine (JVM).
Then load the Java bytecode, which will be executed by the JVM.
*/
void create_vm();

void execute_file(const char* filename, const char* funcname, const char* signature);
void execute_archive(const char* archive, const char* filename, const char* funcname, const char* signature);
void execute_bytecode(const char* bytecode, int length);

//=====================================================================

int main()
{
    // Bytecode string
    const char code[] = "\xca\xfe\xba\xbe\x00\x00\x00\x34\x00\x1c\x0a\x00\x06\x00\x0e\x09"
                        "\x00\x0f\x00\x10\x08\x00\x11\x0a\x00\x12\x00\x13\x07\x00\x14\x07"
                        "\x00\x15\x01\x00\x06\x3c\x69\x6e\x69\x74\x3e\x01\x00\x03\x28\x29"
                        "\x56\x01\x00\x04\x43\x6f\x64\x65\x01\x00\x0f\x4c\x69\x6e\x65\x4e"
                        "\x75\x6d\x62\x65\x72\x54\x61\x62\x6c\x65\x01\x00\x0c\x70\x72\x69"
                        "\x6e\x74\x5f\x73\x74\x72\x69\x6e\x67\x01\x00\x0a\x53\x6f\x75\x72"
                        "\x63\x65\x46\x69\x6c\x65\x01\x00\x0a\x6b\x6c\x61\x7a\x7a\x2e\x6a"
                        "\x61\x76\x61\x0c\x00\x07\x00\x08\x07\x00\x16\x0c\x00\x17\x00\x18"
                        "\x01\x00\x1f\x4d\x65\x74\x68\x6f\x64\x20\x66\x72\x6f\x6d\x20\x6d"
                        "\x65\x6d\x6f\x72\x79\x2d\x6c\x6f\x61\x64\x65\x64\x20\x63\x6c\x61"
                        "\x73\x73\x07\x00\x19\x0c\x00\x1a\x00\x1b\x01\x00\x05\x6b\x6c\x61"
                        "\x7a\x7a\x01\x00\x10\x6a\x61\x76\x61\x2f\x6c\x61\x6e\x67\x2f\x4f"
                        "\x62\x6a\x65\x63\x74\x01\x00\x10\x6a\x61\x76\x61\x2f\x6c\x61\x6e"
                        "\x67\x2f\x53\x79\x73\x74\x65\x6d\x01\x00\x03\x6f\x75\x74\x01\x00"
                        "\x15\x4c\x6a\x61\x76\x61\x2f\x69\x6f\x2f\x50\x72\x69\x6e\x74\x53"
                        "\x74\x72\x65\x61\x6d\x3b\x01\x00\x13\x6a\x61\x76\x61\x2f\x69\x6f"
                        "\x2f\x50\x72\x69\x6e\x74\x53\x74\x72\x65\x61\x6d\x01\x00\x07\x70"
                        "\x72\x69\x6e\x74\x6c\x6e\x01\x00\x15\x28\x4c\x6a\x61\x76\x61\x2f"
                        "\x6c\x61\x6e\x67\x2f\x53\x74\x72\x69\x6e\x67\x3b\x29\x56\x00\x21"
                        "\x00\x05\x00\x06\x00\x00\x00\x00\x00\x02\x00\x01\x00\x07\x00\x08"
                        "\x00\x01\x00\x09\x00\x00\x00\x1d\x00\x01\x00\x01\x00\x00\x00\x05"
                        "\x2a\xb7\x00\x01\xb1\x00\x00\x00\x01\x00\x0a\x00\x00\x00\x06\x00"
                        "\x01\x00\x00\x00\x0f\x00\x09\x00\x0b\x00\x08\x00\x01\x00\x09\x00"
                        "\x00\x00\x25\x00\x02\x00\x00\x00\x00\x00\x09\xb2\x00\x02\x12\x03"
                        "\xb6\x00\x04\xb1\x00\x00\x00\x01\x00\x0a\x00\x00\x00\x0a\x00\x02"
                        "\x00\x00\x00\x13\x00\x08\x00\x14\x00\x01\x00\x0c\x00\x00\x00\x02"
                        "\x00\x0d";


    srand(time(NULL));

    //===== Prepare loading of Java VM =====
    create_vm();

    //===== Execution schemas =====
    // Example 1: Store bytecode as .class and execute it.
    // This will print an addition of two random numbers
    std::cout << "Example 1: Execute function from a file" << std::endl;
    execute_file("file", "add", "(II)I");
    std::cout << std::endl;

    //Example 2: Store .class file in a jar and execute it
    // This will print an addition of two random numbers
    std::cout << "Example 2: Execute function from a file" << std::endl;
    execute_archive("app.jar", "appjar", "add", "(II)I");
    std::cout << std::endl;

    // Example 3: Store bytecode in application and execute it
    // This will print a string to screen
    std::cout << "Example 3: Execute stored bytecode" << std::endl;
    execute_bytecode(code, sizeof(code) - 1);   // without the NULL termination
    std::cout << std::endl;

    //===== Destroy JVM =====
    jvm->DestroyJavaVM();
}

/* Initialize the VM, prepare environment for running Java code */
void create_vm()
{
    JavaVMInitArgs vm_args;                                 // Initialization arguments    
    JavaVMOption  options[2];                               // JVM invokation options
    
    jint version;

    // Set the options for JVM
    options[0].optionString = "-Djava.class.path=.";        // where to find java .class
    options[1].optionString = "-DXcheck:jni:pedantic";      // some JNI stuffs

    // Set the arguments for JVM
    vm_args.version = JNI_VERSION_1_8;                      // Minimum java version 8
    vm_args.nOptions = 2;                                   // Number of options
    vm_args.options = options;                         
    vm_args.ignoreUnrecognized = false;                     // Invalid options make the JVM init fail.

    //===== Load and initialize VM and JNI interface =====
    jint rc = JNI_CreateJavaVM(&jvm, (void**) &env, &vm_args);
    if (rc != JNI_OK || env == nullptr) 
    {   // Error processing...
        std::cout <<  "Unable to launch JVM: " << rc << std::endl;
        exit(1);
    }
    std::cout << "JVM load successfully" << std::endl;
    version = env->GetVersion();
    std::cout << "Version: " << ((version >> 16) & 0x0F) << "." << (version & 0x0F) << std::endl;
    std::cout << std::endl;
}


/* 
Function for Example 1 (.class File)
- Load and import .class file
- Get the attribute to the add() function 
- Create tuple that holds arguments
- Call the function
*/
void execute_file(const char* filename, const char* funcname, const char* signature)
{
    jclass       cls;
    jmethodID    add;
    
    cls = env->FindClass(filename);     // If the class is in package, then use "package/class" notation
    add = env->GetStaticMethodID(cls, funcname, signature);
    if (add == nullptr)
        std::cerr << "method " << funcname << " cannot be found" << std::endl;
    else 
    {
        int args[2] = {rand() + 10, rand() + 9};

        std::cout << "   Args[0] is " << args[0] << std::endl;
        std::cout << "   Args[1] is " << args[1] << std::endl;

        jint res = env->CallStaticIntMethod(cls, add, args[0],args[1]);
        std::cout << "Result of call: " << res << std::endl;
    }
}

/*
Function for Example 2 (archive)
- Add archive to classpath
- Load and import .class file
- Get the attribute to the add() function 
- Create tuple that holds arguments
- Call the function
*/
void execute_archive(const char* archive, const char* filename, const char* funcname, const char* signature)
{
    jvmtiError   error;
    jvmtiEnv*    jvmti_env;

    // Get the JVM TI environment (not usual env)
    jint r = jvm->GetEnv((void **)&jvmti_env, JVMTI_VERSION);
    
    // Add the archive file (.jar) to class loader path
    std::cout << "Adding path \"" << archive << "\" to classpath" << std::endl;
    jvmti_env->AddToSystemClassLoaderSearch(archive);

    // The rest is similar to technnique (1)
    std::cout << "Executing " << filename << "." << funcname << "()" << std::endl;
    execute_file(filename, funcname, signature);
}


/*
Function for Example 3 (bytecode)
- Load the bytecode (if necessary)
- Use classloader to dynamically load classes
*/
void execute_bytecode(const char* bytecode, int length)
{
    jclass       klazz;
    jmethodID    print_string;

    // Create a ClassLoader for loading the class
    jclass loaderClass = env->FindClass("java/lang/ClassLoader"); 
    jmethodID loaderMethod = env->GetStaticMethodID(loaderClass, "getSystemClassLoader", "()Ljava/lang/ClassLoader;");
    jobject loader = env->CallStaticObjectMethod(loaderClass, loaderMethod);

    // Create a class from byte
    klazz = env->DefineClass("klazz", loader, (jbyte*) bytecode, (jint) length);
    if (klazz != nullptr)
    {
        print_string = env->GetStaticMethodID(klazz, "print_string", "()V");
        if (print_string == nullptr)
            std::cerr << "method \"print_string\" cannot be found" << std::endl;
        else 
            env->CallStaticVoidMethod(klazz, print_string);
    }
}

// Reference code
// Dumping classes to byte array using JVM TI
jbyteArray dump_class(JNIEnv* env, jclass cls, jobject method)
{
    jvmtiError      error;
    jvmtiEnv*       jvmti_env;
    jbyteArray      byte_array;
    jint            bytecode_size;
    unsigned char*  bytecodes;

    // Get the JVM TI environment (not usual env)
    jint r = jvm->GetEnv((void **)&jvmti_env, JVMTI_VERSION);

    jmethodID method_id = env->FromReflectedMethod(method);
    if (jvmti_env->GetBytecodes(method_id, &bytecode_size, &bytecodes))
        return NULL;
    
    byte_array = env->NewByteArray(bytecode_size);
    if (byte_array != nullptr)
        env->SetByteArrayRegion(byte_array, 0, bytecode_size, (jbyte*) bytecodes);
    
    jvmti_env->Deallocate(bytecodes);
    return byte_array;
}