#pragma once
#include <string>
#include <vector>
namespace mamba {
    enum class Token {
        Keyword, Number, String, Unknown, Class, Function, Variable, Identifier,
        Constant, Module, Indentation, Assign, Add, Subtract, Multiply, Divide, FloorDivide,
        Modulo, Power, Equal, NotEqual, Less, Greater, LessOrEqual, GreaterOrEqual,
        AddTo, SubtractFrom, MultiplyTo, DivideBy, ModuloBy, OpenParenthesis,
        CloseParenthesis, OpenBracket, CloseBracket, OpenBrace, CloseBrace,
        Comma, Dot, Colon, Semicolon, At
    };
    std::pair<std::string, bool> clearComments(std::string line, bool closedLine = false);
    std::vector<std::pair<const char*, mamba::Token>> buildLexemes(std::string line, bool closedLine = false);
};