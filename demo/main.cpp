//
// Created by Lars on 16/12/2018.
//

#define DEBUG

#include "../LMPA/LMPA.h"
#include "../LMPA/Binary.h"

#include <chrono> // benchmarking

#include "Random.h" // randomized testing

#include <functional>

#include "UnitTests.h"

typedef long long int time_type;

template<typename T, typename... Args>
time_type benchmark(T function, Args&&... args) {
    // call with std::bind to use this on class member functions
    auto starttime = std::chrono::high_resolution_clock::now();
    function(std::forward<Args>(args)...);
    auto endtime = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(endtime - starttime).count();
}

int main() {

//    int a = 264;
//
//    unsigned char b = static_cast<unsigned char>(a);
//
//    std::cout << (int) b << std::endl;
//    std::cout << std::bitset<sizeof(a) * 8>(b) << std::endl;
//    std::cout << std::bitset<sizeof(a) * 8>(a) << std::endl;

//    UnitTests::run();


    //5-byte binary with value 0
//    Binary a(5, true);

    // binary with the value 1234
    size_t b_value = 1234;
    Binary b(b_value);

    // binary with the value 2345
    size_t c_value = SIZE_MAX;
    Binary c(c_value);

    std::cout << "debug" << std::endl;
    b += c;

    int val = static_cast<int>(b);

//    Binary d;
//    d = static_cast<Binary>(17);

//    std::cout << a << std::endl;
    std::cout << b << std::endl;
    std::cout << c << std::endl;
    std::cout << val << std::endl;
    std::cout << b_value + c_value << std::endl;

//    std::cout << d << std::endl;

//    std::cout << std::bitset<sizeof(c) * 8>(c) << std::endl;
//
//    std::cout << c << std::endl;

    return 0;
}
