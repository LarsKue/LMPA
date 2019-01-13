//
// Created by Lars on 27/07/2018.
//
#ifndef RANDOM_LIBRARY_H
#define RANDOM_LIBRARY_H

#include <random>


class Random {
private:
    static std::random_device rd;
    static std::mt19937_64 eng;

public:
    /// non-instantiated class ///
    Random() = delete;
    ~Random() = delete;
    Random(const Random& R) = delete;
    Random& operator=(const Random& R) = delete;

    static void generateSeed();

    static bool get();
    static bool get(double chance);

    static int get(int max);
    static int get(int min, int max);

    static int pm();

};


#endif // RANDOM_LIBRARY_H
