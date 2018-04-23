/**
Calling C++ code form Java

Compile:
    $ javac invoke.java

Create JNI Header:
    $ javah -jni invoke

Run:
    $ java invoke

*/
public class invoke 
{
    private native void print_jni();
    private        void print_java()
    {
        System.out.println("Calling Java from C++ (Lib)");
    }

    public static void main(String[] args)
    {
        // Invoke the "invoke" JNI class
        new invoke().print_jni();
    }
    static 
    {
        // Load the function implemented in native library
        System.loadLibrary("called");
    }
}