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

    UnitTests::run();

    Binary::container_type avec;
    Binary::container_type bvec;
    Binary::container_type cvec;
    Binary::container_type dvec;
    Binary::container_type resultvec;

    Binary::container_type testvec = {1, 1, 1, 0}; // -2

    avec = {1, 1, 0, 0}; // -4
    bvec = {1, 1, 0, 0}; // -4

    cvec = {0, 0, 0, 0, 1, 0, 0, 0, 0}; // 16
    dvec = {0, 0, 1, 0, 0, 1}; // 9

//    resultvec = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};



    Binary a(avec);
    Binary b(bvec);

    Binary c(cvec);
    Binary d(dvec);

    Binary testbin(testvec);

    constexpr std::size_t prec = 8;

    a.reserve(prec);
    b.reserve(prec);

//    constexpr std::size_t n = 1;

//    std::cout << "Operator+= took " << benchmark(
//            [&]() -> void {
//                for (std::size_t i = 0; i < n; ++i) {
//                    a += b;
//                }
//            }
//            ) << " ms." << std::endl;

//    std::cout << std::endl << "------------------------------------------------------------------------" << std::endl;
//    a.print();
//    std::cout << std::endl << "------------------------------------------------------------------------" << std::endl;
//    b.print();

//    a.shrink_to_fit();
//    a.print();
//
//    std::cout << a << std::endl;
//    std::cout << "Precision: " << a.precision() << std::endl;


//    Binary result(resultvec);

//    std::cout << a + b << "\t= 0b10011" << std::endl;
//    std::cout << a - b << "\t= 0b00001" << std::endl;
//    std::cout << std::endl;
//    std::cout << c + d << "\t= 0b11001" <<std::endl;
//    std::cout << c - d << "\t= 0b00111" << std::endl;

    // 19 - 25
//    result = (a+b) - (c+d);
//
//    std::cout << result << "\t= -0b000000000110" << std::endl;
//
//    // -6 + 19
//    result += a + b;
//
//    std::cout << result << "\t= 0b000000001101" << std::endl;
//
//    // 10 + 13, new precision: 13
//    a += result;



//    std::cout << a << "\t= 0b000000010111" << std::endl;
//
//    constexpr int n = 10000;
//    constexpr int maxlen = 10000;
//
//    Random::generateSeed();
//
//    std::vector<Binary> bins;
//
//    auto time1 = std::chrono::high_resolution_clock::now();
//
//    for (int i = 0; i < n; ++i) {
//        // generate randomized length of Binary
//        int len = Random::get(1, maxlen);
//        // create randomized vector of said length
//        std::vector<bool> vec;
//        for (int j = 0; j < len; ++j) {
//            vec.push_back(Random::get());
//        }
//        // initialize binary with said vector
//        bins.emplace_back(Binary(vec));
//    }
//
//    Binary result;
//
//    auto time2 = std::chrono::high_resolution_clock::now();
//
//    for (const auto& bin : bins) {
//        result += bin;
//    }
//
//    auto endtime = std::chrono::high_resolution_clock::now();
//    auto constructiontime = std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count();
//    auto executiontime = std::chrono::duration_cast<std::chrono::milliseconds>(endtime - time2).count();
//
//    std::cout << result << std::endl;
//
//    std::cout << "\nExecution time: " << executiontime << " ms." << std::endl;
//    std::cout << "\nConstruction time: " << constructiontime << " ms." << std::endl;





//    srand(time(0));

//    for (int i = 0; i < 100; ++i) {
//        avec.clear();
//        bvec.clear();
//        for (int j = 0; j < 5; ++j) {
//            avec.push_back(rand() % 2);
//            bvec.push_back(rand() % 2);
//        }
//        Binary a(avec);
//        Binary b(bvec);
//
//        std::cout << a << std::endl;
//        std::cout << b << std::endl;
//        std::cout << "a + b: " << a + b << std::endl;
//        std::cout << "a - b: " << a - b << std::endl;
//
//        std::cout << "--------------------------------------------------" << std::endl;
//    }

    return 0;
}
