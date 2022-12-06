/* Mamba Python by Vladyslav Korol | Apache License 2
 * This file implements the built-in string module used to
 * process, manipulate and and otherwise work with strings. */

#include "string.hh"

namespace mamba::string {
    /// @brief Splits the string, capitialises every word and joins them together.
    /// @param string The input string to process.
    /// @param sep The seperator to divide by.
    /// @return The amalgamation of all cap words.
    mamba::String capwords(const mamba::String& string, const mamba::String& sep = " ") {
        unsigned int start{}, end{};
        mamba::String result;
        int separator = string.find(sep);
        while (separator != -1) {
            end = separator;
            result += string.substring(start, end).capitalize();
            result += sep;
            start = end + sep.size();
            separator = string.find(sep, start);
        }
        result += string.substring(start).capitalize();
        return result;
    }


};