/* Mozilla Public License, v2 | Vladyslav Korol
 * This file contains all the reusable pieces of code
 * that are needed at different stages of deduction.*/
#include <string>
#include <map>
namespace Interpreter {
    //The static lexemes:
    const std::string keywords[] = {"and", "as", "assert", "break", "class", "continue",
        "def", "del", "elif", "else", "except", "False", "finally", "for", "from", "global",
        "if", "import", "in", "is", "lambda", "None", "not", "or", "pass", "raise",
        "return", "True", "try", "while", "with", "yield"};
    //Complex operators must come before the simple ones in order to extract them first.
    const std::string operators[] = {"+=", "-=", "*=", "/=", "+", "-", "**", "*", 
                "//", "/", "%", "==", "!=", "=", "<=", ">=", "<", ">",};
    const std::string separators[] = {"(", ")", "[", "]", "{", "}", ",", ".", ":", ";", "@"};
    /// @brief The function used to convert const char* to arbitrary integer to switch them.
    /// @return A unique arbitrary constant expression integer that can be used in switch cases.
    constexpr unsigned int str2int(const char* str, int h = 0){
        return !str[h] ? 5281 : (str2int(str, h+1) * 33) ^ str[h];
    }
    //The reusable ids:
    enum Token {
        Keyword, Operator, Separator, Number, String, Unknown,
        Class, Function, Object, Constant, Module, Indentation
    };
    /// @brief The composite type of an individual lexeme
    /// as a token with certain type and varying metadata.
    typedef struct Lexeme {
        Token type;
        std::string label;
        std::map<std::string, std::string> attributes;
    } Lexeme;
    //The string-to-lexeme identifier table to keep track of all user definitions.
    std::map<std::string, Lexeme> identifiers;
};