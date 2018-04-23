/**
Calling C++ code form .NET CLR

Compile:
    $ csc /out:invoke.exe invoke.cs

Run:
    $ invoke
*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace InteropInvoker
{
    class Invoke 
    {
        [DllImport("called.dll")]
        private static extern void print_clr(string str);

        static void Main(string[] args)
        {
            print_clr("P/Invoke");
        }
    }
}