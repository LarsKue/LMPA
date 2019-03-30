//
// Created by Lars on 16/12/2018.
//

#define LMPA_DEBUG
#define LMPA_OVERFLOW_PROTECTION

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
    std::size_t a_value = 1234;
    std::size_t b_value = 7558126;

    Binary a(a_value);
    Binary b(b_value);

    std::cout << a << std::endl;
    std::cout << b << std::endl;
    a += b;
    std::cout << a << std::endl;

    return 0;
}
