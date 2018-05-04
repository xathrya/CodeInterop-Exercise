/**
Calling C++ code form .NET CLR

Compile:
    $ csc /out:invokeCS.exe invoke.cs

Run:
    $ invokeCS
*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

/*
    P/Invoke
    Call the native code from managed code.
    The prototype of native function should be declared before use.

    Reverse P/Invoke
    Call the managed code from native code.
    Usually as a callback
    A delegate function should be declared before use.
    Delegate function will then forward the call to specific function.
*/

namespace InteropInvoker
{
    class Invoke 
    {
        //===== Native declaration ====================================
        [DllImport("called.dll")]
        private static extern void print_clr(string str);

        [DllImport("called.dll")]
        public static extern void dispatch(string str, int count, callback call);
        
        //===== Delegate declaration ==================================
        // Used for Reverse P/Invoke
        public delegate void callback(string str);

        //===== Callback declaration ==================================
        // This function will be called by delegate function
        public static void worker(string str)
        {
            System.Console.WriteLine("Called C#: " + str);
        }

        //===== Main functions ========================================
        static void Main(string[] args)
        {
            // P/Invoke
            // Call print_clr() on called.dll
            print_clr("P/Invoke");

            // Reverse P/Invoke
            // Call dispatch() on called.dll which then it will call the callback()
            // the delegate will then forward the call to .Net function of worker()
            dispatch("Reverse P/Invoke", 10, new callback(Invoke.worker));
        }
    }
}