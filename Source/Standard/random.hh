/* Declares the Python random module used for generating random values.
 * Documentation source: docs.python.org/3/library/random.html
 * To implement: getstate(), setstate(), betavariate(), expovariate(),
   gammavariate(), gauss(), longnormvariate(), normalvariate(), 
   vonmisesvariate(), paretovariate(), weibullvariate(), Random, SystemRandom */
#include <iostream>
#include "./Interpreter/Base.hh"

namespace mamba::random {
    //Bookkeeping functions:  
    /// @brief Initialises a random number generator.
    /// @param a The seed value to hook on.
    void seed(const unsigned int a, const int version = 2);
    
    //Random generators:
    
    /// @brief Generates n random bytes.
    mamba::Bitset randbytes(unsigned int n); 
    
    /// @brief Generates a random integer between 0 to stop inclusively.
    int randrange(int stop);
    
    /// @brief Generates a random integer from start exclusively to stop inclusively with step.
    int randrange(const int start, const int stop, const int step = 1);
    
    /// @brief Generates a random integer more than a but smaller than b.
    int randint(int a, int b);
    
    unsigned int getrandbits(unsigned int k);
     
    /// @brief Selects and returns a random item from the sequence.
    mamba::Bitset choice(mamba::Bitset sequence);
    
    /// @brief Returns k amount of random values from the population.
    mamba::Bitset choices(mamba::Bitset population, unsigned int k = 1);
    
    /// @brief Puts the elements of Python sequence in random order.
    void shuffle(mamba::Bitset x);
    
    /// @brief Returns a k-sized list of unique elements in the population.
    mamba::Bitset sample(mamba::Bitset population, unsigned int k);
    
    /// @brief Returns a random number from [0.0, 1.0).
    mamba::Float random();
    
    /// @brief Returns a random floating-point number that is more
    /// than the lesser value and smaller than the bigger one.
    float uniform(int a, int b);
    
    /// @brief Returns a random float between low and high.
    float triangular(int low, int high);
    
    
    
    
};
