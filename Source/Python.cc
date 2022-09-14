/* Mozilla Public License, v2 | Vladyslav Korol
 * The core of the interpreter that takes the source 
 * input and makes the series of external calls in the
 * namespace to execute the code. The app works in CLI. */
#include <iostream>
#include <fstream>
#include "CLI.cc"
#include "Deduction/Inclusion.hpp"
#include "Execution/Inclusion.hpp"
namespace Interpreter {
    /* The heart of the interpreter. It is built as
     * a command line interface tool and unites all 
     * components into read-execute-repeat loop.*/
    /// @brief Executes a single line of code in REP loop.
    /// @param line The stringline to be executed.
    void execute(std::string line){}

    int main(){
        std::string line;
        std::ifstream source("Samples/TypeConversion.py");
        if (source.is_open()){
            while (getline(source, line)) execute(line);
            source.close();
        }
        return 0;
    }
};