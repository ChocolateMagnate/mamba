//This file only exposes common declarations needed for initial compilation:
//  1. The mamba::Token enum class;
//  2. The mamba::str2int function;
//  3. The mamba::Lexeme struct;
//  4. Keywords, operators, separators and indentations arrays.
#include <string>
#include <map>
namespace mamba {
    //The static lexemes:
    const char keywords[][9] = {"and", "as", "assert", "break", "class", "continue",
        "def", "del", "elif", "else", "except", "False", "finally", "for", "from", "global",
        "if", "import", "in", "is", "lambda", "None", "nonlocal", "not", "or", "pass", "raise",
        "return", "True", "try", "while", "with", "yield"};
    //Complex operators must come before the simple ones in order to extract them first.
    const char operators[][3] = {"+=", "-=", "*=", "/=", "+", "-", "**", "*", 
                "//", "/", "%", "==", "!=", "<=", ">=", "=", "<", ">",};
    const char separators[][2] = {"(", ")", "[", "]", "{", "}", ",", ".", ":", ";", "@"};
    const char indentations[][5] = {"   ", "\t"};
    /// @brief The function used to convert const char* to arbitrary integer to switch them.
    /// @return A unique arbitrary constant expression integer that can be used in switch cases.
    constexpr unsigned int str2int(const char* str, int h = 0){
        return !str[h] ? 5281 : (str2int(str, h+1) * 33) ^ str[h];
    }
    enum Token {
        Keyword, Number, String, Unknown, Class, Function, Variable, Identifier,
        Constant, Module, Indentation, Assign, Add, Subtract, Multiply, Divide, FloorDivide,
        Modulo, Power, Equal, NotEqual, Less, Greater, LessOrEqual, GreaterOrEqual,
        AddTo, SubtractFrom, MultiplyTo, DivideBy, ModuloBy, OpenParenthesis,
        CloseParenthesis, OpenBracket, CloseBracket, OpenBrace, CloseBrace,
        Comma, Dot, Colon, Semicolon, At
    };
    /// @brief The composite type of an individual lexeme
    /// as a token with certain type and varying metadata.
    typedef struct Lexeme {
        int type;
        std::string label;
    } Lexeme;
};