//
// Created by Lars on 16/12/2018.
//

#define LMPA_DEBUG
#define LMPA_LAZY_MULTIPLICATION

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


// TODO: A bunch of testing with negative values
int main() {
    int a_value = -2500;
    int b_value = 2000;

    Binary a(a_value);
    a.promote(8);
    Binary b(b_value);
    b.promote(8);

    std::cout << a << std::endl;

    a *= b;

    std::cout << a << std::endl;

    std::cout << INT_MIN << std::endl;
    std::cout << static_cast<int64_t>(a) << std::endl;

    return 0;
}
