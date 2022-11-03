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
            std::cout << "Match : " << match.str() << "\n"; 
            line = match.suffix();
            lexemes.push_back({match.str().c_str(), mamba::Token::Unknown});
        }
        return lexemes;
    }
};