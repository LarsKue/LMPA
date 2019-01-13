#include "Random.h"

/// Standard Libraries ///
#include <chrono>

/// Statics ///
std::random_device Random::rd;
std::mt19937_64 Random::eng;


/// generates a random seed for the mersenne twister engine based on the current system time
void Random::generateSeed() {
    auto seed = static_cast<unsigned long long>(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    eng.seed(seed);

    // warm up the seed
    for (int i = 0; i < 20; ++i) {
        std::uniform_real_distribution<double> dist(0, 1);
        dist(eng);
    }
}

/// returns true the specified percentage of the time
bool Random::get(double chance) {
    std::uniform_real_distribution<double> dist(0, 1);
    return dist(eng) <= chance;
}

/// returns true 50% of the time
bool Random::get() {
    return Random::get(0.5);
}

/// returns a random int between 0 and max
int Random::get(int max) {
    std::uniform_int_distribution<int> dist(0, max);
    return dist(eng);
}

/// returns a random int between min and max
int Random::get(int min, int max) {
    if (min == max) {
        return min;
    }
    else if (min > max) {
        // swap values
        int temp = min;
        min = max;
        max = temp;
    }
    std::uniform_int_distribution<int> dist(min, max);
    return dist(eng);
}

/// returns -1 or 1, 50% of the time
int Random::pm() {
    if (Random::get()) {
        return 1;
    }
    return -1;
}