/**
Calling Java code from C++

Compile:
    $ javac called.java
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

// See the signature with `javap -s -p caled`
public class called
{
    private static int magic_counter = 135;
    private        int uid;

    // Type signature: ()V
    public called()
    {
        uid = magic_counter++ * 2;
    }

    // Type signature: ()Ljava/lang/String;
    public String showId()
    {
        return "My ID is " + uid;
    }

    // Type signature: ()I
    public static int    magic()
    {
        return 135;
    }
    // Type signature: (II)V
    public static void   multi(int x, int y)
    {
        System.out.println("I was called with " + x + " and " + y);
    }
    // Type signature: ([Ljava/lang/String;)V
    public static void hello(String[] names)
    {
        System.out.println("Hello,");
        for (String name : names)
        {
            System.out.println("  " + name);
        }
    }
}