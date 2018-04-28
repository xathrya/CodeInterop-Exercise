### Reversing.ID Code-Interop
* Invoker: C++
* Called: C# (.Net)
* Technique: COM Interop

Build:
- Build the called as DLL
- Build the invoker
- Register the DLL to generates equivalent type library in COM environments
    $ regasm
- Run the invoker