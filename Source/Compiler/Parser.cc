
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "Compiler.hh"
#include "Parser.hh"

namespace mamba {
    /* When the compiler parsed the source text into the flow of tokens,
     * the next step is always parsing the grammars in the code by the rules
     * permitted by the language. At the end, an Abstract Syntax Tree is 
     * generated that furthermore is converted into the bytecode. All sorts
     * of errors and warnings are also raised on this stage. */
    
    enum class TokenRelation {
        Addition, Substraction, Multiplication, TrueDivision, FloorDivision, 
        Assign, AddTo, SubtractFrom, MultiplyBy, DivideBy, Access,/*the dot operator*/
        Import, 
    }
    /// @brief The base AST class used to represent the flow of the program.
    class AbstractSyntaxTree {
        unsingned int size; //Indicates the total size of lexemes.
        Lexeme *start, *counter;
    public:
        AbstractSyntaxTree(unsigned int capacity = 400){
            this->size = 0;
            this->start = (Lexeme*)malloc(capacity);
            this->counter = this->start;
        }
        ~AbstractSyntaxTree() {
            this->size = 0;
            free(this->start);
        }
        /// @brief Appends a lexeme as a child of its parent.
        Lexeme* child(const Lexeme* parent, const Lexeme& lexeme, const TokenRelation relation) {
            
            return (Lexeme*)this->counter;
        }
    }

    /// @brief Prints the compile-time warning into the terminal.
    /// @param message The warning message to be printed.
    /// @param line The line number where the warning occured.
    /// @param offset The column in the line above.
    void warn(const char* message, const char* line, int offset) {
        std::cout << line << "|" << line << "\n"
                << std::string(offset, ' ') << "^\n" << message << std::endl;
    }

    /// @brief Raises the compile-time error and terminates the program.
    /// @param message The error message to be printed.
    /// @param line The line number where the error occured.
    /// @param offset The column in the line above.
    void raiseError(const char* message, const char* line, const int offset) {
        std::cout << line << "|" << line << "\n"
                << std::string(offset, ' ') << "^\n" << message << std::endl;
        exit(1);
    }
};
