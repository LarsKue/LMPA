//
// Created by Lars on 16/12/2018.
//
#include "../LMPA/LMPA.h"
#include "../LMPA/Binary.h"

#include <chrono> // benchmarking

#include "Random.h" // randomized testing

#include <functional>

#include "UnitTests.h"

template<typename T, typename... Args>
double benchmark(T function, Args&&... args) {
    // call with std::bind to use this on class member functions
    auto starttime = std::chrono::high_resolution_clock::now();
    function(std::forward<Args>(args)...);
    auto endtime = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(endtime - starttime).count();
}

int main() {

    constexpr std::size_t n = 100000;
    constexpr std::size_t tries = 25;

    UnitTests::run();

    double result = 0;

    for (std::size_t j = 0; j < tries; ++j) {

        double time = benchmark(
                [&]() -> void {
                    // 50000
                    Binary a({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0});
                    // 5
                    Binary b({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1});
                    a.reserve(256);
                    b.reserve(256);

                    for (std::size_t i = 0; i < n; ++i) {
                        a * b;
//                        std::cout << "Progress: " << i  << " out of " << n << "\r" << std::endl;
                    }
                }
        );

        std::cout << time << std::endl;
        result += time;

    }

    std::cout << "Average: " << result / tries << std::endl;

    return 0;
}
