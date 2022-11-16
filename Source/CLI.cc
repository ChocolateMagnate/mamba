/* Mamba Python by Vladyslav Korol | Apache License 2.0 
 * This file takes care of command line integration and 
 * outputing various messages in response to commands.*/
#include <iostream>
#include <fstream>
#include <string>
#include "Commons.cc"
#define trigger "mamba" //The word that is recognised as the CLI trigger.
namespace mamba {
    /// @brief The welcome message printed when the interpreter is triggered.
    void welcome(){
        std::cout << "Academic Python v1 Copyright Vladyslav Korol (@ChocolateMagnate)\n" <<
            "This work is licensed under Mozilla Public License v2.\n" <<
            "The application is a completely functional interpreter for the Python programming language" <<
            "that supports all of its features, including:\n" << "* dynamic typisation\n" << 
            "* custom functions and classes\n" << "* linkage with the entirety of Python ecosystem" <<
            "* integration with C and C++ APIs and libraries\n" << "* improved performance compared to CPython.\n" <<
            "This interpreter was created for the academic exercising purposes and an extra challenge" <<
            "during the author's Python curriculum.\n" << "In order to get help, type academic-python -h." <<
            "To interpret a Python file, type: academic-python [PATH]" << std::endl;
            
    }
    /// @brief Prints the help message explaining how to use the tool and what commands it accepts.
    void help(){
        std::cout << "Usage: academic-python [OPTIONS]\n" << "OPTIONS:\n" <<
            "[PATH] -- Interpreters the input file. Must be surrounded in quotation marks.\n" <<
            "--show-lexemes -- Prints the list of recognised lexemes.\n" <<
            "--show-bytecode -- Prints the generated cached bytecode.\n" <<
            "--list-dependencies -- Prints the list of used libraries.\n" <<
            "--script [PATH] --input -- Executes a script that requires another file to process." << std::endl;
    }
    /// @brief Prints the detailed instruction how to use a single command.
    /// @param option The option in the command line to explain. 
    void helpDescribe(std::string option){
        switch(str2int(option.c_str())){  //We use str2int function to convert const char* to integral
            case str2int("show-lexemes")://pre-calculated value because strings cannot be switched.
                std::cout << "For the debugging purposes, this function is used to print " <<
                    "all the lexemes recognised in the code and make sure they were labelled " << 
                    "correctly." << std::endl;
                break;
            case str2int("show-bytecode"):
                std::cout << "Since it takes time to parse the source code and furthermore to " <<
                    "execute it, the interpretation thought has evolved into producing a machine-" <<
                    "friendly intermediate representation that conveys the whole code and can be " <<
                    "natively executed by the interpreter. Academic Python produces its own bytecode " <<
                    "and caches it in order to not parse the whole file again and reuse it. When " << 
                    "changes happen in the code, the bytecode is edited to the extend of changes. It is " <<
                    "built as a sequence of integers for the ease of representation and matching." << std::endl;
                break;
            case str2int("list-dependencies"):
                std::cout << "Any application is likely to use external building components " <<
                    "to simplify the job that would be much harder to implement every other time. " <<
                    "This command lists all the libraries that are imported into the code as well as " <<
                    "the items used in them, such as classes, constants, functions and the language." << std::endl;
                break;
            default:
                std::cout << "This command does not exist. Use academic-python -h to get help or " <<
                    "academic-python [PATH] to execute a Python app." << std::endl;
                break;
        }
    }
    /// @brief Prints the lexemes recognised in the source code. It is 
    /// used as a flag in conjugation with the execution function.
    void showLexemes(){
        //for (auto lexeme : identifiers) //<< operator is not overloaded
        //    std::cout << lexeme.first << ", " << lexeme.second << std::endl;
    }
    /// @brief Prints the list of all dependencies and their items used in the app.
    void listDependencies(){}
    /// @brief Prints the array of integers used as the IR to quickly execute the app.
    void showBytecode(){}
};