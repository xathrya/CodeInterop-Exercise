# Calling C code from Python using CTypes
# Invoke the exported functions
#
# Reference: https://docs.python.org/3/library/ctypes.html
#
# Run:
#    $ python invoke.py
from ctypes import *
import os

# Default to Windows DLL in CTypes
# from ctypes import windll, cdll
#    windll.kernel32
#    windll.shell32
#    ctypes.msvcrt

### Load the library
if os.name == 'nt':
    # Windows
    called = cdll.LoadLibrary("./called.dll")
else:
    # Linux and other
    called = cdll.LoadLibrary("./called.so")

### Simple call
called.iop_print(b"Interop operation with CTypes\n")
print("Add result in %d" % called.iop_add(100, 35))

# Value map
# ctypes type   |  C type              |   Python type
#---------------+----------------------+-----------------------------
# c_bool        |  _Bool               |   bool (1)
# c_char        |  char                |   1-character string
# c_wchar       |  wchar_t             |   1-character unicode string
# c_byte        |  char                |   int/long
# c_ubyte       |  unsigned char       |   int/long
# c_short       |  short               |   int/long
# c_ushort      |  unsigned short      |   int/long
# c_int         |  int                 |   int/long
# c_uint        |  unsigned int        |   int/long
# c_long        |  long                |   int/long
# c_ulong       |  unsigned long       |   int/long
# c_longlong    |  __int64             |   int/long 
#               |  long long           |
# c_ulonglong   |  unsigned __int64    |   int/long
#               |  unsigned long long  |  	
# c_float       |  float               |   float
# c_double      |  double              |   float
# c_longdouble  |  long double         |   float
# c_char_p      |  char *              |   string or None
# c_wchar_p     |  wchar_t *           |   unicode or None
# c_void_p      |  void *              |   int/long or None
# 

print("Return type is: " + str(called.iop_add.restype))

### Define structure
class Vector(Structure):
    _fields_ = [
        ("x", c_int),
        ("y", c_int),
        ("z", c_int)
    ]
print("Types and offset of Vector:\nx={}\ny={}\nz={}".format(Vector.x, Vector.y, Vector.z))

### Invoking function with structure type
vector_a = Vector(1, 2, 3)
vector_b = Vector(2, 3, 4)
called.add_vector.restype = Vector
vector_c = called.add_vector(vector_a, vector_b)
print("a + b = ({}, {}, {})".format(vector_c.x, vector_c.y, vector_c.z))