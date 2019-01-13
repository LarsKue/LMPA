# LMPA
Lightweight Multiple Precision Arithmetic Library for C++, for Windows and UNIX

This Library uses strings to extend upon the precision of primitive arithmetic types in C++.

Currently supported are:
1. Integer and floating point types of any length up to std::string::max digits (2^63 - 1 on most systems)
2. Arithmetic operations + and -
3. Flexible precision
4. Easy output of numbers

Planned for future support are:
1. Arithmetic operations * and / as well as %
2. Logical operations
3. Assignment operations


**Installation (using CMake):**
1. Download this repository
2. Build this repository using CMake
3. Copy LMPA/LMPA.h into the include directory of your project
4. Copy LMPA/libLMPA.a from the build directory into the libraries directory of your project
5. Link the Library to your project
6. Build your project
