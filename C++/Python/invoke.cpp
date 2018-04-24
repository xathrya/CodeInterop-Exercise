/**
Calling Python code from C++
Tested on:
    - Python 3.6.4 (x64) and Ms. Visual Studio 2017 Community Ed.

All experiment is using execution schema "load script file"
See also "schema.cpp" inside directory "schemas" for other schemas.

Include:
    - $PYTHON/include

Lib:
    - $PYTHON/libs/

Compile:
    (GCC)
    $ g++ invoke.cpp -o invoke -I$PYTHON/include -L$PYTHON/libs -lpython36 -std=c++11

    (MSVC)
    $ cl invoke.cpp /I $PYTHON/include $PYTHON/libs/python36.lib 

Run:
    $ invoke

*/
#include <iostream>
#include <Python.h>

//========================== Static & Globals ==========================
wchar_t * program;


//========================== Helper  Function ==========================

void vm_init(const char* argv0);
void vm_cleanup();

void call_return_int(PyObject* module, const char* funcname);
void call_list_args(PyObject* module, const char* funcname);
void call_multi_args(PyObject* module, const char* funcname);
void call_multi_ret(PyObject* module, const char* funcname);
void instantiate_class(PyObject* module, const char* classname);

//=====================================================================

int main(int argc, char* argv[])
{
    PyObject *name, *module;

    //===== Prepare loading of Python =====
    vm_init(argv[0]);

    //===== Loading the file =====
    name = PyUnicode_DecodeFSDefault("called"); // Convert from filename to a Python string
    module = PyImport_Import(name);             // Import the file as Python module
    Py_DECREF(name);

    if (module == nullptr)
    {
        std::cerr << "Cannot find module" << std::endl;
        PyErr_Print();
        exit(1);
    }
    
    // Example 1: call method that return integer
    // Variation: function is an object which is stored in dictionary
    std::cout << "Example 1: Function that return integer" << std::endl;
    call_return_int(module, "random_magic");
    std::cout << std::endl;
    
    // Example 2: call method that require array of string
    std::cout << "Example 2: Function that requre array of string" << std::endl;
    call_list_args(module, "hello");
    std::cout << std::endl;
    
    // Example 3: call method that require multiple argument
    std::cout << "Example 3: Function that require multiple argument" << std::endl;
    call_multi_args(module, "add");
    std::cout << std::endl;
    
    // Example 4: call method that return multiple return values
    std::cout << "Example 5: Function that return multiple values" << std::endl;
    call_multi_ret(module, "split_string");
    std::cout << std::endl;
    
    // Example 5: instantiate a class by constructor
    std::cout << "Example 4: Instantiate a class" << std::endl;
    instantiate_class(module, "called");
    std::cout << std::endl;

    Py_DECREF(module);
        

    //===== Clean up Python VM =====
    vm_cleanup();

    return 0;
}

void vm_init(const char* argv0)
{
    program = Py_DecodeLocale(argv0, NULL);
    if (program == nullptr)
    {
        std::cerr << "Fatal error: cannot decode " << argv0 << std::endl;
        exit(1);
    }
    Py_SetProgramName(program);
    Py_Initialize();
}

void vm_cleanup()
{
    if (Py_FinalizeEx() < 0)
        exit(120);
    
    PyMem_RawFree(program);
}

/*
Function for Example 1 (Return integer)
- Case the return value to integer
*/
void call_return_int(PyObject* module, const char* funcname)
{
    PyObject *dict, *func, *args, *value;

    // Create a dictionary for the contents of the module
    dict = PyModule_GetDict(module);

    // Get the add method from the dictionary  
    func = PyDict_GetItemString(dict, funcname);
    if (func && PyCallable_Check(func))
    {
        // Call the function and received the return value
        // Python function always need
        //    1. Pointer to python function
        //    2. Pointer to argument list (tuple)
        // If no argument for function, pass empty tuuple
        args = PyTuple_New(0);
        
        value = PyObject_CallObject(func, args);
        Py_DECREF(args);
        if (value == nullptr)
        {
            std::cerr << "Cannot call the function " << funcname << std::endl;
            PyErr_Print();
        }
        else 
        {
            std::cout << "Result of call: " << PyLong_AsLong(value) << std::endl;
            Py_DECREF(value);
        }
        Py_DECREF(func);
    }
    else 
    {
        std::cerr << "Cannot find function " << funcname << std::endl;
        if (PyErr_Occurred())
            PyErr_Print();
    }
    Py_DECREF(dict);
}

/*
Function for Example 2 (list of string as argument)
Argument is a tuple thus list of string is an argument.
- Create a tuple
- Create and populate a list
- Set the list as content of tuple

*/
void call_list_args(PyObject* module, const char* funcname)
{
    #define ELEMCOUNT 2     // Initial size, can be extended
    PyObject *func, *args, *list, *value;
    int i;

    // Assume a module is an object an each object has attribute.
    // Get the attribute from object and return it as function
    func = PyObject_GetAttrString(module, funcname);
    if (func && PyCallable_Check(func))
    {
        // Call the function and received the return value
        // Python function always need
        //    1. Pointer to python function
        //    2. Pointer to argument list (tuple)
        // Make sure to define the length corresponding to the amount of argument
        args = PyTuple_New(1);

        // Create a list and populate it
        list = PyList_New(ELEMCOUNT);

        // Set the allocated item
        PyList_SetItem(list, 0, PyLong_FromLong(13510));
        PyList_SetItem(list, 1, PyUnicode_FromString("Xathrya"));

        // Append, insert new item from behind
        for (i = 0; i < 10; i++)
            PyList_Append(list, PyLong_FromLong((long)i));

        // Insert new item from position (0-based index)
        PyList_Insert(list, 0, PyUnicode_FromString("ReversingID"));
        
        // Set the list to the tuple (or arguments)
        PyTuple_SetItem(args, 0, list);
        
        PyObject_CallObject(func, args);
        Py_DECREF(args);
    }
    else 
    {
        std::cerr << "Cannot find function " << funcname << std::endl;
        if (PyErr_Occurred())
            PyErr_Print();
    }
    Py_DECREF(func);
}

/*
Function for Example 3 (multiple arguments)
Argument is a tuple thus adding element of tuple means add more argument.
- Create a tuple 
- Set item of tuple
*/
void call_multi_args(PyObject* module, const char* funcname)
{
    #define ARGCOUNT 2
    PyObject *func, *args, *value;
    int i, tmp_arg;

    // Assume a module is an object an each object has attribute.
    // Get the attribute from object and return it as function
    func = PyObject_GetAttrString(module, funcname);
    if (func && PyCallable_Check(func))
    {
        // Call the function and received the return value
        // Python function always need
        //    1. Pointer to python function
        //    2. Pointer to argument list (tuple)
        // Make sure to define the length corresponding to the amount of argument
        args = PyTuple_New(ARGCOUNT);

        for (i = 0; i < ARGCOUNT; i++)
        {
            tmp_arg = rand() - i + 10;
            std::cout << "   Args[" << i << "] is " << tmp_arg << std::endl;

            // Assign the tuple with integers
            value = PyLong_FromLong(tmp_arg);
            if (!value)
            {
                Py_DECREF(args);
                Py_DECREF(func);
                std::cerr << "Cannot convert arguments" << std::endl;
                return;
            }
            PyTuple_SetItem(args, i, value);
        }
        
        value = PyObject_CallObject(func, args);
        Py_DECREF(args);
        if (value != nullptr)
        {
            std::cout << "Result of call: " << PyLong_AsLong(value) << std::endl;
            Py_DECREF(value);
        }
        else 
        {
            std::cerr << "Cannot call the function " << funcname << std::endl;
            PyErr_Print();
        }
    }
    else 
    {
        std::cerr << "Cannot find function " << funcname << std::endl;
        if (PyErr_Occurred())
            PyErr_Print();
    }
    Py_DECREF(func);
}

/*
Function for Example 4 (return multiple value)
Multiple return values is actually an object with tuple of other object.
To obtain all object: first determine it's length
- Determine the length of tuple 
- Case each item to appropriate type
*/
void call_multi_ret(PyObject* module, const char* funcname)
{
    PyObject *func, *args, *value;
    int i;

    // Assume a module is an object an each object has attribute.
    // Get the attribute from object and return it as function
    func = PyObject_GetAttrString(module, funcname);
    if (func && PyCallable_Check(func))
    {
        // Call the function and received the return value
        // Python function always need
        //    1. Pointer to python function
        //    2. Pointer to argument list (tuple)
        // Make sure to define the length corresponding to the amount of argument
        args = PyTuple_New(1);

        // Set the argument for calling function
        PyTuple_SetItem(args, 0, PyUnicode_FromString("Reversing.ID Community Indonesia"));

        // Call the functions
        PyObject* list = PyObject_CallObject(func, args);
        Py_DECREF(args);
        if (list != nullptr)
        {
            const char * retval;
            int length = PyList_Size(list);
            
            std::cout << "Returning " << length << " elements" << std::endl;

            for (i = 0; i < length; i++)
            {
                // Get the item
                value = PyList_GetItem(list, i);
                if (!PyUnicode_Check(value))
                {
                    Py_DECREF(list);
                    Py_DECREF(func);
                    std::cerr << "Return items must be string" << std::endl;
                    return;
                }

                // Print the item
                // Our string is unicode so we need to decode it to bytes
                PyObject* str  = PyUnicode_AsEncodedString(value, "utf-8", "~E~");
                retval = PyBytes_AsString(str); 
                std::cout << "   RetVal[" << i << "] is " << retval << std::endl;
                Py_DECREF(str);
            }
            Py_DECREF(list);
        }
        else 
        {
            std::cerr << "Cannot call the function " << funcname << std::endl;
            PyErr_Print();
        }        
    }
    else 
    {
        std::cerr << "Cannot find function " << funcname << std::endl;
        if (PyErr_Occurred())
            PyErr_Print();
    }
    Py_DECREF(func);
}

/*
Function for Example 5 (instantiate class)
- Load the reference to class
- Load the constructor (or __init__)
- Call the constructor and supply necessary parameters
*/
void instantiate_class(PyObject* module, const char* classname)
{
    PyObject *klass, *args, *instance, *method;

    // Get the reference to the class
    klass = PyObject_GetAttrString(module, classname);

    // Instantiate
    // Our class has not parameter so we will give NULL
    args = Py_BuildValue("()");
    instance = PyObject_CallObject(klass, args);
    Py_DECREF(args);

    //// Alternative
    // 
    
    // Calling instance's method
    method = PyObject_GetAttrString(instance, "showId");
    args   = Py_BuildValue("()");
    PyEval_CallObject(method, args);
    
    Py_DECREF(klass);
    Py_DECREF(method);
    Py_DECREF(args);    
    Py_DECREF(instance);

    /* Alternative ================
    // Directly instance class without parameter
    instance = PyObject_CallObject(klass, NULL);

    // Directly call instance's method without parameter
    PyObject_CallMethod(instance, "showId", NULL);
    */
}