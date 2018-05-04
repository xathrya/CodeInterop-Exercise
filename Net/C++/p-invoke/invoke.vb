' Calling C++ code from .NET CLR
'
' Compile: 
'   $ vbc /out:invokeVB.exe invoke.vb
'
' Run:
'   $ invokeVB
imports System
imports System.Runtime.InteropServices

' P/Invoke
' Call the native code from managed code.
' The prototype of native function should be declared before use.

' Reverse P/Invoke
' Call the managed code from native code.
' Usually as a callback
' A delegate function should be declared before use.
' Delegate function will then forward the call to specific function.

public class Invoke 

    '===== Native declaration ====================================
    ' Both of this type of declarations are valid
    <DllImport("called.dll", EntryPoint:="print_clr", CallingConvention:=CallingConvention.StdCall)>
    private shared  sub print_clr(str as string)
    end sub

    public  declare sub dispatch lib "called.dll" (str as string, count as integer, [call] as Invoke.callback)

    '===== Delegate declaration ==================================
    ' Used for Reverse P/Invoke
    public delegate sub callback(str as string)

    '===== Callback declaration ==================================
    ' This function will be called by delegate function
    public shared sub worker(str as string)
        System.Console.WriteLine("Called VB: " & str)
    end sub


    '===== Main functions ========================================
    public shared sub Main()    
        ' P/Invoke
        ' Call print_clr() on called.dll
        print_clr("P/Invoke")

        ' Reverse P/Invoke
        ' Call dispatch() on called.dll which then it will call the callback()
        ' the delegate will then forward the call to .Net function of worker()
        dispatch("Reverse P/Invoke", 10, AddressOf Invoke.worker)
    end sub 'Main
end class   'Invoke