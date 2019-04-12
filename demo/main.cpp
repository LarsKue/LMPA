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

#include <thread>

typedef long long int time_type;

template<typename T, typename... Args>
time_type benchmark(T function, Args&&... args) {
    // call with std::bind to use this on class member functions
    auto starttime = std::chrono::high_resolution_clock::now();
    function(std::forward<Args>(args)...);
    auto endtime = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(endtime - starttime).count();
}

std::vector<std::thread*> threads;

void test_addition(int64_t a_value) {
    for (int64_t b_value = -10000; b_value <= 10000; ++b_value) {
        Binary a(a_value);
        a.promote(8);
        Binary b(b_value);
        b.shrink();

        a += b;

        if (static_cast<int64_t>(a) != a_value + b_value) {
            std::cerr << "Error on A = " << a_value << " B = " << b_value << std::endl;
            std::cerr << "A:                " << Binary(a_value) << std::endl;
            std::cerr << "B:                " << b << std::endl;
            std::cerr << "Binary Result:    " << a << std::endl;
            std::cerr << "Expected Result:  " << "0b" << std::bitset<sizeof(a_value) * 8>(a_value + b_value) << std::endl;
            std::cerr << "Decimal Results:  " << std::endl;
            std::cerr << "Expected " << a_value + b_value << " but got " << static_cast<int64_t>(a) << std::endl;
            throw std::runtime_error("A: " + std::to_string(a_value) + "; B: " + std::to_string(b_value));
        }
    }
    std::cout << "Finished Testing A = " << a_value << std::endl;
}

void test_multiplication(int64_t a_value) {
    for (int64_t b_value = -10000; b_value <= 10000; ++b_value) {
        Binary a(a_value);
        a.promote(8);
        Binary b(b_value);
        b.shrink();

        a *= b;

        if (static_cast<int64_t>(a) != a_value * b_value) {
            std::cerr << "Error on A = " << a_value << " B = " << b_value << std::endl;
            std::cerr << "A:                " << Binary(a_value) << std::endl;
            std::cerr << "B:                " << b << std::endl;
            std::cerr << "Binary Result:    " << a << std::endl;
            std::cerr << "Expected Result:  " << "0b" << std::bitset<sizeof(a_value) * 8>(a_value * b_value) << std::endl;
            std::cerr << "Decimal Results:  " << std::endl;
            std::cerr << "Expected " << a_value * b_value << " but got " << static_cast<int64_t>(a) << std::endl;
            throw std::runtime_error("A: " + std::to_string(a_value) + "; B: " + std::to_string(b_value));
        }
    }
    std::cout << "Finished Testing A = " << a_value << std::endl;
}


// TODO: A bunch of testing with negative values
int main() {

//    int64_t a_value = 1;
//    int64_t b_value = 1;
//
//    Binary a(a_value);
//    Binary b(b_value);
//    b.shrink();
//
//    a *= b;
//
//    std::cout << a_value * b_value << std::endl;
//    std::cout << static_cast<int64_t>(a) << std::endl;
//    std::cout << "0b" << std::bitset<64>(static_cast<unsigned long long int>(a_value * b_value)) << std::endl;
//    std::cout << a << std::endl;


    for (int64_t a_value = -10000; a_value <= 10000; ++a_value) {
        test_multiplication(a_value);
    }
    return 0;
}
