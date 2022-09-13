/* GNU Lesser General Public License | Vladyslav Korol
 * This file divides the Python source code into a table
 * of lexemes: keywords, ids, etc. for further parsing.*/
#include <iostream>
#include <string>
#include <vector>
#include <map>
namespace Interpreter {
    /* Tokenisation is always the first step done
     * in any programming language. It deals with
     * removing comments and extracting different
     * tokens from the code such as keywords, 
     * operators, separators, and user-defined 
     * identifiers. Once the code is presented as
     * steam of lexemes, it can be grammatically 
     * matched into a syntax tree and be executed.*/
    enum Token {
        Keyword, Operator, Separator, Number, String, Unknown,
        Class, Function, Variable, Constant, Module, Indentation
    };
    /// @brief The composite type of an individual lexeme
    /// as a token with certain type and varying metadata.
    typedef struct Lexeme {
        Token type;
        std::string label;
        std::map<std::string, std::string> attributes;
    } Lexeme;
    


};