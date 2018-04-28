/**
Calling .Net code form C++ (native)

Create a strong name
    $ sn -k assembly-key.snk

Compile:
    $ csc /t:library /out:CalledDll.dll CalledDll.cs

Register (need administrator privilege):
    $ regasm CalledDll.dll /tlb:CalledDll.tlb /codebase

ps: see the generated CalledDll.tlh for types
*/
using System;
using System.Reflection;
using System.Runtime.InteropServices;

// Assembly Info
// Registering COM interface require the DLL to have strong name
[assembly: AssemblyTitle("CalledDll")]
[assembly: AssemblyDescription("")]
[assembly: AssemblyConfiguration("")]
[assembly: AssemblyCompany("")]
[assembly: AssemblyProduct("CalledDll")]
[assembly: AssemblyCopyright("Copyright ©  2018")]
[assembly: AssemblyTrademark("")]
[assembly: AssemblyCulture("")]
[assembly: AssemblyVersion("1.0.0.0")]
[assembly: AssemblyFileVersion("1.0.0.0")]
[assembly: ComVisible(true)] 
[assembly: AssemblyDelaySign(false)] 
[assembly: AssemblyKeyFile("assembly-key.snk")]

namespace CalledDll
{
    // Declare interface 
    public interface ICalled
    {
        int  magic();
        void hello(string[] names);
        void multi(int x, int y);
    }

    // Implement interface
    public class Called : ICalled
    {
        public Called()
        { }

        // Example 1: method that return integer
        public int magic()
        {
            return 135;
        }

        // Example 2: method that require array of string
        public void hello(string[] names)
        {
            System.Console.WriteLine("Hello, ");
            foreach (string name in names)
            {
                System.Console.WriteLine("  " + name);
            }
        }

        // Example 3: method that require multiple argument
        public void multi(int x, int y)
        {
            System.Console.WriteLine("I was called with " + x + " and " + y);
        }
    }
}
