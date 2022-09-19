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
    
    /// @brief Interprets the input line into machine-readable IR. 
    /// @return The array of integers to pass to the execute function.
    std::vector<int> interpret(std::string line){
        auto code = parseComponents(splitIntoComponents(clearComments(line)));
        return generateIR(parseGrammar(code));
    }
    /// @brief Executes a single line of code in REP loop.
    /// @param query The pointer to the array of integers to be matched.
    void execute(std::vector<int> query){
        for (int command : query){
            switch (command){}
        }
    }

    int main(){
        std::string line;
        std::ifstream source("Samples/TypeConversion.py");
        if (source.is_open()){
            while (getline(source, line)) execute(interpret(line));
            source.close();
        }
        return 0;
    }
};