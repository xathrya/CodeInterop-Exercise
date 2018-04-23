### Reversing.ID Code-Interop
* Invoker: C++
* Called: Java
* Technique: JNI

Invocation Steps:
- Create Java VM with JNI_CreateJavaVM()
- Load class with FindClass()
- Obtain method signature with GetStaticMethodID() for static method
- Call the method
- Destroy the JVM

Build:
- Build the called
- Build the invoker
- Run the invoker