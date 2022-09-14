/* Mozilla Public License, v2 | Vladyslav Korol
 * This file contains all the reusable pieces of code
 * that are needed at different stages of deduction.*/
#include <string>
#include <map>
namespace Interpreter {
    const std::string keywords[] = {"and", "as", "assert", "break", "class", "continue",
        "def", "del", "elif", "else", "except", "False", "finally", "for", "from", "global",
        "if", "import", "in", "is", "lambda", "None", "nonlocal", "not", "or", "pass", "raise",
        "return", "True", "try", "while", "with", "yield"};
    //Complex operators must come before the simple ones in order to extract them first.
    const std::string operators[] = {"+", "-", "*", "**", "//", "/",  "%", "==","!=", "=", "<=", ">=", "<", ">",};
    const std::string separators[] = {"(", ")", "[", "]", "{", "}", ",", ".", ":", ";", "@"};
    
};