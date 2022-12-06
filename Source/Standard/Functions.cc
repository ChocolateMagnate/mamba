/* Mamba Python by Vladyslav Korol | Apache License 2
 * This file implements the built-in Pythonic functions
 * that do not need explicit imports, including len, zip, etc.*/
#pragma once
#include <cmath>
#include "Functions.hh"

namespace mamba {
    /// @brief Returns the absolute value of the number.
    unsigned int abs(mamba::Bitset& x) {
        return abs(x.toInt());
    }

    /// @brief Returns True if any element of the iterable is true.
    /// If the iterable is empty, returns False.
    bool any(mamba::Bitset iterable) {
        for (auto cell : iterable) if (!cell.toBool()) return false;
        return true;
    }

    /// @brief Prints the object to the standard output.
    /// @param text The object (integer, string, custom object) to print.
    /// @param separator If multiple objects are provided, this is the separator.
    /// @param end The character to print at the end of the function.
    /// @param flush The flag to indicate whether to flush the output (std::endl).
    void print(mamba::Bitset& text, const char* separator = " ",
                const char* end = "\n", bool flush = false) {
        std::cout << text.toString() << separator << end;
        if (flush) std::cout << std::endl;
    }