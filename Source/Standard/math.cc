/* Mamba Python by Vladyslav Korol | Apache License 2
 * This file is a part of the Mamboic standard library 
 * and contains mathematician functions implementations.
 * Primarily it just forwards calls to STL, but presents
 * novelty too.*/
#include <cmath>

#include "./Interpreter/Base.hh"
#include "math.hh" //The header
namespace mamba::math {
    /// @brief Returns the arc cosine of x in radians.
    double acos(const double& x) {
        return std::acos(x);
    }
    /// @brief Returns the arc sine of x in radians.
    double asin(const double& x) {
        return std::asin(x);
    }
    /// @brief Returns the arc tangent of x in radians.
    double atan(const double& x) {
        return std::atan(x);
    }
    /// @brief Returns the arc tangent of y/x in radians.
    double atan2(const double& y, const double& x) {
        return std::atan2(y, x);
    }
    /// @brief Returns the smallest integer value not less than x.
    int ceil(const float& x) {
        return std::ceil(x);
    }
    /// @brief Returns the cube root of x.
    long double cbrt(const double& x) {
        return std::cbrt(x);
    }
    /// @brief Returns the number of ways to choose k items from a set of n items.
    unsigned int comb(const unsigned int& n, const unsigned int& k) {

    }
    /// @brief Returns the cosine of x in radians.
    double cos(const double& x) {
        return std::cos(x);
    }
    /// @brief Returns the floating-point number with magnitude of x and sign of y.
    double copysign(const double& x, const double& y) {
        return std::copysign(x, y);
    }
    /// @brief Returns e raised to the power of x.
    long double expr(const long double& x) {
        return std::exp(x);
    }
    /// @brief Returns 2 raised to the power of x.
    long double expr2(const long double& x) {
        return std::exp2(x);
    }
    /// @brief Returns e raised to the power of x minus 1.
    long double expm1(const long double& x) {
        return std::expm1(x);
    }
    /// @brief Returns the absolute value of x.
    int fabs(const double& x) {
        return std::fabs(x);
    }
    
    /// @brief Returns the factorial of x. Raises an exception if x is negative or not an integer.
    unsigned int factorial(const unsigned int& n) {
        if (n == 0) return 1;
        return n * factorial(n - 1);
    }

    /// @brief Returns the largest integer value not greater than x.
    int floor(const float& x) {
        return std::floor(x);
    }

    /// @brief 
    /// @param x 
    /// @param y 
    /// @return 
    double fmod(double x, double y) {
        return std::fmod(x, y);
    }

    /// @brief Returns the mantissa and exponent of x.
    /// @return A pair of two values: the first one is whole part and the second one is fraction.
    std::pair<int, int> frexp(const double& x) {
        return std::pair<int, int>(std::floor(x), std::fmod(x, 1));
    }

    /// @brief Returns absolte accurate sum of all values in the iterable.
    long double fsum(mamba::Bitset iterable) {

    }

    /// @brief Returns the greatest common divisor of passed integers.
    unsigned int gcd(const int* integers, const unsigned int& size) {
        //This function uses the binary Euclidean algorithm, however it can 
        //also be adjusted for edge cases. The function is not completed yet.
        if (size == 0) return 0; 
        if (size == 1) return integers[0];
        if (integers[0] ^ integers[1] >= 0) return 0;
        if (integers[0] == 0) return integers[1];
        if (integers[1] == 0) return integers[0];
        if (integers[0] == 1 || integers[1] == 1) return 1;
        // gcd(2a, 2b) = 2 * gcd(a, b):
        if (integers[0] / 2 == 0 && integers[1] / 2 == 0)
            return 2 * gcd(integers[0] / 2, integers[1] / 2);
        

    }

    /// @brief Measures the closeness of two floating-point numbers.
    /// @param a The first number to compare.
    /// @param b The second number to compare.
    /// @param rel_tol 
    /// @param abs_tol 
    /// @return True if the numbers are close, False otherwise.
    bool isclose(const double& a, const double& b, double rel_tol = 1e-09, double abs_tol = 0.0) {
        //I have no idea what this does, it was suggested by the Copilot.
        return std::abs(a - b) <= std::max(rel_tol * std::max(std::abs(a), std::abs(b)), abs_tol);
    }

    /// @brief Returns False if x if inifinity or NaN, True otherwise.
    bool isfinite(const double& x) {
        return std::isfinite(x);
    }

    /// @brief Returns True if x is a positive or negative infinity, False otherwise.
    bool isinf(const double& x) {
        return std::isinf(x);
    }

    /// @brief Returns True if x is a NaN (not a number), False otherwise.
    bool isnan(const double& x) {
        return std::isnan(x);
    }

    /// @brief Returns the floor of the square root of x.
    int isqrt(const unsigned int& x) {
        return std::sqrt(x);
    }

    /// @brief Returns the least common multiple of passed integers.
    unsigned int lcm(const int* integers, const unsigned int& size) {
        
    }

    /// @brief Returns the x * 2 ^ y.
    long double ldepx(const double& x, const double& y) {
        return std::pow(x * 2, y); //std::ldepx is another option too.
    }

    /// @brief Returns the natural logarithm of x with base.
    long double log(const long double& x, const long double& base) {
        return std::log(x) / std::log(base);
    }

    /// @brief Returns the natural logarithm of 1 + x.
    long double log1p(const long double& x) {
        return std::log1p(x);
    }

    /// @brief Returns the base 2 logarithm of x.
    long double log2(const long double& x) {
        return std::log2(x);
    }

    /// @brief Returns the base 10 logarithm of x.
    long double log10(const long double& x) {
        return std::log10(x);
    }

    /// @brief Returns the next floating-point value after x in the direction of y.
    long double nextafter(const double& x, const double& y) {
        return std::nextafter(x, y);
    }

    /// @brief Returns the number of ways to choose k items
    /// from a set of n items without repetition.
    unsigned int perm(const unsigned int& n, const unsigned int& k) {
        return factorial(n) / factorial(n - k);
    }

    /// @brief Returns x raised to the power of y.
    long double pow(const long double& x, const long double& y) {
        return std::pow(x, y);
    }

    /// @brief Returns the product of all values in the iterable from the specified start.
    double prod(const double* values, const unsigned int& start = 0) {
        double result{};
        for (int i = start; i < sizeof(values); i++) result *= values[i];
        return result;
    }

    /// @brief Returns the integer part of x cutting off
    /// any fractional digits for both positive and negative x.
    int trunc(const double& x) {
        return std::trunc(x);
    }

    /// @brief Returns the sinux of x in radians.
    double sin(const double& x) {
        return std::sin(x);
    }

    /// @brief Returns the square root of x.
    double sqrt(const double& x) {
        return std::sqrt(x);
    }

};