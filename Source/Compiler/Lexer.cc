/* Mozilla Public License, v2 | Vladyslav Korol
 * This file divides the Python source code into a table
 * of lexemes: keywords, ids, etc. for further parsing.*/
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <list>
#include <map>
//#include <boost/algorithm/string.hpp>
#include "/workspaces/python-interpreter/Source/Commons.cc"
#define append push_back
#define substring substr
using mamba::str2int;
using namespace std;
namespace mamba {
    /* Tokenisation is always the first step done
     * in any programming language. It deals with
     * removing comments and extracting different
     * tokens from the code such as keywords, 
     * operators, separators, and user-defined 
     * identifiers. Once the code is presented as
     * steam of lexemes, it can be grammatically 
     * matched into a syntax tree and be executed.*/
    unsigned int indentationCounter = 0;
    bool isnumber(const char* str){
        char character = str[0];
        while (character != '\0'){
            if (character < '0' || character > '9') return false;
            character = str[++character];
        }
        return true;
    }
    /// @brief Cleanses tag comments from the line.
    pair<string, bool> clearComments(string line, bool closedLine = false){
        // Step 1. Clear unclosed multiline comment.
        if (closedLine) {
            int end = line.find("\"\"\"");
            if (end != string::npos) line = line.substring(end + 3);
            else return {"", false};
        }
        int tag = line.find("#");
        if (tag != string::npos) return {line.substring(0, tag), true};
        int triple = line.find("\"\"\"");
        if (triple != string::npos) return clearComments(line, true);
        return {line, closedLine};
    }
    /// @brief Takes a line of code and tokenises it into a list of lexemes.
    /// @param line The standard string to be tokenised.
    /// @return Vector of pairs of const char* and the token type.
    std::vector<std::pair<const char*, mamba::Token>> buildLexemes(std::string line){
        std::vector<std::pair<const char*, mamba::Token>> lexemes;
        lexemes.resize(20); //Lexemes is an offset array.
    /* The regular expression matches:
        1. \"\"\"\"[^\"\"\"]*\"\"\"\" any triple string;
        2. \"[^\"]*\" any double string;
        3. '[^']*' any single string;
        4. [\(\){}[[\].\-+:;<>?!&*%@=]+ any number of operators and separators;
        5. [^\(\){}[\].\-+:;<>?!&*%@=]+ any number of code that is not special characters;
        6. [+-]?([0-9]+([.][0-9]*)?|[.][0-9]+)] any floating-point number. */
        const char expression[] = R"(\"\"\"\"[^\"\"\"]*\"\"\"\"|\"[^\"]*\"|'[^']*'|[+-]?([0-9]+([.][0-9]*)?|[.][0-9]+)]|[\(\){}[[\].\-+:;<>?!&*%@=]+|[^\(\){}[\].\-+:;<>?!&*%@=]+)";
        std::regex pattern(expression);
        std::smatch match;
        while (std::regex_search(line, match, pattern)) {
            auto fragment = match.str().c_str();
            std::cout << "Match : " << fragment << "\n"; 
            line = match.suffix();
            if (fragment[0] == '\"' || fragment[0] == '\''||
                    fragment[1] == '\"'|| fragment[1] == '\'') {
                lexemes.append({fragment, mamba::Token::String});
                continue;
            }
            if (isnumber(fragment)) {
                lexemes.append({fragment, mamba::Token::Number});
                continue;
            } 
            mamba::Token token = mamba::Token::Identifier;
            switch (str2int(fragment)){
                //Keywords:
                case str2int("and"): case str2int("as"): case str2int("assert"):
                case str2int("break"): case str2int("class"): case str2int("continue"):
                case str2int("def"): case str2int("del"): case str2int("elif"):
                case str2int("else"): case str2int("except"): case str2int("False"):
                case str2int("finally"): case str2int("for"): case str2int("from"):
                case str2int("global"): case str2int("if"): case str2int("import"):
                case str2int("in"): case str2int("is"): case str2int("lambda"):
                case str2int("None"): case str2int("nonlocal"): case str2int("not"):
                case str2int("or"): case str2int("pass"): case str2int("raise"):
                case str2int("return"): case str2int("True"): case str2int("try"):
                case str2int("while"): case str2int("with"): case str2int("yield"):
                    token = mamba::Token::Keyword; break;
                //Operators:
                case str2int("+="): 
                    token = mamba::Token::AddTo; break;
                case str2int("-="):
                    token = mamba::Token::SubtractFrom; break;
                case str2int("*="):
                    token = mamba::Token::MultiplyTo; break;
                case str2int("/="):
                    token = mamba::Token::DivideBy; break;
                case str2int("%="):
                    token = mamba::Token::ModuloBy; break;
                case str2int("!="):
                    token = mamba::Token::NotEqual; break;
                case str2int("=="):
                    token = mamba::Token::Equal; break;
                case str2int("<="):
                    token = mamba::Token::LessOrEqual; break;
                case str2int(">="):
                    token = mamba::Token::GreaterOrEqual; break;
                case str2int("**"):
                    token = mamba::Token::Power; break;
                case str2int("//"):
                    token = mamba::Token::FloorDivide; break;
                case str2int("+"):
                    token = mamba::Token::Add; break;
                case str2int("-"):
                    token = mamba::Token::Subtract; break;
                case str2int("*"):
                    token = mamba::Token::Multiply; break;
                case str2int("/"):
                    token = mamba::Token::Divide; break;
                case str2int("%"):
                    token = mamba::Token::Modulo; break;
                case str2int("="):
                    token = mamba::Token::Assign; break;
                case str2int("<"):
                    token = mamba::Token::Less; break;
                case str2int(">"):
                    token = mamba::Token::Greater; break;
                //Separators:
                case str2int("("):
                    token = mamba::Token::OpenParenthesis; break;
                case str2int(")"):
                    token = mamba::Token::CloseParenthesis; break;
                case str2int("{"):
                    token = mamba::Token::OpenBrace; break;
                case str2int("}"):
                    token = mamba::Token::CloseBrace; break;
                case str2int("["):
                    token = mamba::Token::OpenBracket; break;
                case str2int("]"):
                    token = mamba::Token::CloseBracket; break;
                case str2int("."):
                    token = mamba::Token::Dot; break;
                case str2int(":"):
                    token = mamba::Token::Colon; break;
                case str2int(","):
                    token = mamba::Token::Comma; break;
                case str2int("@"):
                    token = mamba::Token::At; break;
                case str2int(";"):
                    token = mamba::Token::Semicolon; break;
            }
            lexemes.push_back({match.str().c_str(), token});
        }
        return lexemes;
    }
};