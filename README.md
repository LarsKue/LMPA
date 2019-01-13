# LMPA
Lightweight Multiple Precision Arithmetic Library for C++, for Windows and UNIX

This Library uses standard containers to extend upon the precision of primitive arithmetic types in C++.

**Only dependent on the STL!**


**Currently supported are:**
1. Binary numbers of any length up to std::size_t::max (2^64 - 1 on most systems)
2. Flexible Container types for said Binaries
3. Almost all Standard Arithmetic Operations and Assignment Operations
4. All Standard Logical Operations
5. Easily changeable precision of Binaries
6. Easy Output of Binaries

**Planned for future support are:**
1. Complete Support for all Arithmetic Operations
2. Binary-Based Multi-Precision Integers and Floating Point Numbers


**Installation (using CMake):**
1. Download this repository
2. Build this repository using CMake
3. Copy all header files from LMPA/ into the include directory of your project
4. Copy LMPA/libLMPA.a from the build directory into the libraries directory of your project
5. Link the Library to your project
6. Build your project
