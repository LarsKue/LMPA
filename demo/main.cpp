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

    UnitTests::run();


    // 5-bit binary with value 0
    Binary a(5, true);

    // binary with the value 1234
    Binary b(1234);

    int c = static_cast<int>(b);

    Binary d;
    d = static_cast<Binary>(17);


    std::cout << std::bitset<sizeof(c) * 8>(c) << std::endl;

    std::cout << c << std::endl;

//    a.print();
//    b.print();

    return 0;
}
