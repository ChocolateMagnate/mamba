/* Mamba Python by Vladyslav Korol | Apache License 2
 * THis file implements error handling in the runtime with exceptions.*/
#include <iostream>
#include <vector>

#include "Interpreter/Base.hh"
#include "./Exceptions.hh" //The header

namespace mamba {

    /// @brief Issues a warning message and continues the execution.
    /// @param warning 
    void warn(unsigned int warning) {}

    /// @brief Raises an exception specified by the argument.
    /// @param exception Enum class member denoting the exception to raise.
    /// @param line The line that caused the exception.
    /// @param offset The column in the line that caused the exception.
    /// @param length The length of the token that caused the exception.
    void raise(unsigned int exception, int line, int offset, int length) {
        //To do: implement the elegant call stack printing.
        std::cout << "-------------------------------------" << 
            "Traceback (most recent call last):\n" <<
            exceptionDescriptions[static_cast<int>(exception)] << "\n" <<
            line << "\n|" << std::string(offset, ' ') << std::string(length, '^') << "\n" <<
            "-------------------------------------" << std::endl;
        exit(1);

    }

    const char** exceptionDescriptions = {};

    const char** warningDescriptions = {};

    std::vector<std::tuple<unsigned int, const char*, const char*>> customExceptions = {};

    /// @brief Adds a developer-defined exception as a post-38 integer and attached metadata.
    /// @param name The name of the exception, for example PandasError or LookupError.
    /// @param description The text to display when the exception occurs.
    /// @return The integer ID of the exception.
    unsigned int addCustomException(const char* name, const char* description) {
        customExceptions.emplace_back(customExceptions.size() + 38, name, description);
        return customExceptions.size() + 38;
    }
};