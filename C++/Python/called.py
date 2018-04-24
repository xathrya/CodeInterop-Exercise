# Calling Python code from C++
# Tested on:
#     - Python 3.6.4 (x64) and Ms. Visual Studio 2017 Community Ed.
# 
#

# Example 1: Method that return integer
def random_magic():
    return 135

# Example 2: Method that require array of string
def hello(names):
    print("Hello, ")
    for name in names:
        print("  {}".format(name))

# Example 3: Method that require multiple argument
def add(a, b):
    return a+b
 
# Example 4: Method that return multiple values
def split_string(compound):
     return compound.split()

# Example 5: Instantiate a class by constructor
class called:
    magic = 135

    def __init__(self):
        print("Instantiate")
    
    def showId(self):
        print("My ID is {}".format(self.magic))