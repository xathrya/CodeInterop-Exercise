/**
Calling Java code from C++
Technique: execute function on file

This file will be loaded by technique.exe and then the method add() 
will be called

Compile:
    $ javac file.java
*/

/*
ref: http://journals.ecs.soton.ac.uk/java/tutorial/native1.1/implementing/method.html
Type Signature Table List
Signature    Java Type
Z            boolean
B            byte
C            char
S            short
I            int
J            long
F            float
D            double
V            void
L            fully-qualified-class ;    fully-qualified-class
[ type   type[]


*/

// See the signature with `javap -s -p file`
public class file
{
    // Type signature: (II)I
    public static int    add(int a, int b)
    {
        return a + b;
    }

    public static void main(String[] args)
    {
        System.out.println("Add 100 and 35 = " + add(100, 35));
    }
}