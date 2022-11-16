#pragma once
#include <cmath>
#include <vector>
namespace mamba::math {
    /// @brief Returns the arc cosine of x in radians.
    double acos(const double& x);

    /// @brief Returns the arc sine of x in radians.
    double asin(const double& x);

    /// @brief Returns the arc tangent of x in radians.
    double atan(const double& x);  

    /// @brief Returns the arc tangent of y/x in radians.
    double atan2(const double& y, const double& x);

    /// @brief Returns the smallest integer value not less than x.
    int ceil(const float& x);

    /// @brief Returns the cube root of x.
    long double cbrt(const double& x);

    /// @brief Returns the number of ways to choose k items from a set of n items.
    unsigned int comb(const unsigned int& n, const unsigned int& k);

    /// @brief Returns the cosine of x in radians.
    double cos(const double& x);

    /// @brief Returns the floating-point number with magnitude of x and sign of y.
    double copysign(const double& x, const double& y);

    /// @brief Returns e raised to the power of x.
    long double expr(const long double& x);

    /// @brief Returns 2 raised to the power of x.
    long double expr2(const long double& x);

    /// @brief Returns e raised to the power of x minus 1.
    long double expm1(const long double& x);

    /// @brief Returns the absolute value of x.
    int fabs(const double& x);

    /// @brief Returns the factorial of x. Raises an exception if x is negative or not an integer.
    unsigned int factorial(const unsigned int& n);

    /// @brief Returns the largest integer value not greater than x.
    int floor(const float& x);

    /// @brief 
    /// @param x 
    /// @param y 
    /// @return 
    double fmod(double x, double y);

    /// @brief Returns the mantissa and exponent of x.
    /// @return A pair of two values: the first one is whole part and the second one is fraction.
    std::pair<int, int> frexp(const double& x);

    /// @brief Returns absolte accurate sum of all values in the iterable.
    long double fsum();

    /// @brief Returns the greatest common divisor of passed integers.
    unsigned int gcd(const std::vector<int>& integers);

    /// @brief Measures the closeness of two floating-point numbers.
    /// @param a The first number to compare.
    /// @param b The second number to compare.
    /// @param rel_tol 
    /// @param abs_tol 
    /// @return True if the numbers are close, False otherwise.
    bool isclose(const double& a, const double& b, double rel_tol = 1e-09, double abs_tol = 0.0);

    /// @brief Returns False if x if inifinity or NaN, True otherwise.
    bool isfinite(const double& x);

    /// @brief Returns True if x is a positive or negative infinity, False otherwise.
    bool isinf(const double& x);

    /// @brief Returns True if x is a NaN (not a number), False otherwise.
    bool isnan(const double& x);

    /// @brief Returns the floor of the square root of x.
    int isqrt(const unsigned int& x);

    /// @brief Returns the least common multiple of passed integers.
    unsigned int lcm(const std::vector<int>& integers);

    /// @brief Returns the x * 2 ^ y.
    long double ldepx(const double& x, const double& y);

    /// @brief Returns the natural logarithm of x with base.
    long double log(const long double& x, const long double& base);

    /// @brief Returns the natural logarithm of 1 + x.
    long double log1p(const long double& x);

    /// @brief Returns the base 2 logarithm of x.
    long double log2(const long double& x);

    /// @brief Returns the base 10 logarithm of x.
    long double log10(const long double& x);

    /// @brief Returns the next floating-point value after x in the direction of y.
    long double nextafter(const double& x, const double& y); 

    /// @brief Returns the number of ways to choose k items
    /// from a set of n items without repetition.
    unsigned int perm(const unsigned int& n, const unsigned int& k);

    /// @brief Returns x raised to the power of y.
    long double pow(const long double& x, const long double& y);

    /// @brief Returns the product of all values in the iterable from the specified start.
    double prod(const std::vector<double>& values, const unsigned int& start = 0);

    /// @brief Returns the integer part of x cutting off
    /// any fractional digits for both positive and negative x.
    int trunc(const double& x);

    /// @brief Returns the sinux of x in radians.
    double sin(const double& x);

    /// @brief Returns the square root of x.
    double sqrt(const double& x);
    
};