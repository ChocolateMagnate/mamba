/* Mamba Python by Vladyslav Korol | Apache License 2
 * This file implements Python random module used to 
 * generate random literal values. */
#include <cstdlib>
#include "random.hh"
namespace mamba::random {

    volatile int __seed;
    void init() {
        __seed = time(NULL);
    }

    /// @brief Initialises a random number generator.
    /// @param a The seed value to hook on.
    void seed(const unsigned int a, const int version = 2) {
        __seed = a;
    }

    /// @brief Generates a random integer between 0 to stop inclusively.
    int randrange(int stop) {
        return rand() % stop;
    }

    /// @brief Generates a random integer from start exclusively to stop inclusively with step.
    int randrange(const int start, const int stop, const int step = 1) {
        return rand() % (stop - start) + start;
    }

    /// @brief Generates a random integer more than a but smaller than b.
    int randint(int a, int b) {
        return rand() % (b - a) + a;
    }

    /// @brief Returns a random floating-point number that is more
    /// than the lesser value and smaller than the bigger one.
    float uniform(int a, int b) {
        return (float)rand() / (float)(RAND_MAX / (b - a)) + a;
    }
    
    /// @brief Returns a random float between low and high.
    float triangular(int low, int high) {
        return (float)rand() / (float)(RAND_MAX / (high - low)) + low;
    }
};