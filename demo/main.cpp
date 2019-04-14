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

template<typename T>
int get_progress(T current, T min, T max) {
    return (100 * (current - min)) / (max - min);
}

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
//    std::cout << "Finished Testing A = " << a_value << std::endl;
}


void test_integral_multiplication(int64_t a_value) {
    for (int64_t b_value = -10000; b_value <= 10000; ++b_value) {
        Binary a(a_value);
        a.promote(8);
        a *= b_value;

        if (static_cast<int64_t>(a) != a_value * b_value) {
            std::cerr << "Error on A = " << a_value << " B = " << b_value << std::endl;
            std::cerr << "A:                " << Binary(a_value) << std::endl;
            std::cerr << "B:                " << "0b" << std::bitset<sizeof(b_value) * 8>(b_value) << std::endl;
            std::cerr << "Binary Result:    " << a << std::endl;
            std::cerr << "Expected Result:  " << "0b" << std::bitset<sizeof(a_value) * 8>(a_value * b_value) << std::endl;
            std::cerr << "Decimal Results:  " << std::endl;
            std::cerr << "Expected " << a_value * b_value << " but got " << static_cast<int64_t>(a) << std::endl;
            throw std::runtime_error("A: " + std::to_string(a_value) + "; B: " + std::to_string(b_value));
        }
    }
}

void test_integral_addition(int64_t a_value) {
    for (int64_t b_value = -10000; b_value <= 10000; ++b_value) {
        Binary a(a_value);
        a.promote(8);

        a += b_value;

        if (static_cast<int64_t>(a) != a_value + b_value) {
            std::cerr << "Error on A = " << a_value << " B = " << b_value << std::endl;
            std::cerr << "A:                " << Binary(a_value) << std::endl;
            std::cerr << "B:                " << "0b" << std::bitset<sizeof(b_value) * 8>(b_value) << std::endl;
            std::cerr << "Binary Result:    " << a << std::endl;
            std::cerr << "Expected Result:  " << "0b" << std::bitset<sizeof(a_value) * 8>(a_value + b_value) << std::endl;
            std::cerr << "Decimal Results:  " << std::endl;
            std::cerr << "Expected " << a_value + b_value << " but got " << static_cast<int64_t>(a) << std::endl;
            throw std::runtime_error("A: " + std::to_string(a_value) + "; B: " + std::to_string(b_value));
        }
    }
}


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

    using value_type = int64_t;

    value_type iterate_from = -10000;
    value_type iterate_to = 10000;

    value_type progress = 0;

    int num_threads = 1;

    for (value_type a_value = iterate_from; a_value <= iterate_to; /* no increment */) {
        for (int i = 0; i < num_threads; ++i) {
            auto* t = new std::thread(test_integral_addition, a_value++);
            threads.emplace_back(t);
        }
        for (auto* t : threads) {
            t->join();
            delete t;
        }
        threads.clear();
        if (progress < get_progress<value_type>(a_value, iterate_from, iterate_to)) {
            std::cout << ++progress << "\r";
        }
    }

    return 0;
}
