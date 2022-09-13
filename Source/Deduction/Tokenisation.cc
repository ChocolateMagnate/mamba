/* GNU Lesser General Public License | Vladyslav Korol
 * This file divides the Python source code into a table
 * of lexemes: keywords, ids, etc. for further parsing.*/
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include "Commons.cc"
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

    /// @brief Cleanses tag comments from the line.
    std::string clearComments(std::string line){
        int tag = line.find("#");
        if (tag != std::string::npos) return line.substr(0, tag);
    }

    /// @brief Splits the line into a linked list of individual
    /// meaningful parts that are ready to be parsed as lexemes.
    std::list<std::string> splitIntoComponents(std::string line){
        std::list<std::string> components;
        //Step 1. Extract the strings.
        int quote = line.find("\"");
        while (quote != std::string::npos){
            int end = line.find("\"", quote+1);
            if (end == std::string::npos) break;
            components.push_back(line.substr(quote, end-quote+1));
            line = line.substr(end+1);
            quote = line.find("\"");
        }
        //Step 2. Remove the whitespaces.
        int whitespace = line.find(" ");
        while (whitespace != std::string::npos){
            components.push_back(line.substr(0, whitespace));
            line = line.substr(whitespace+1);
            whitespace = line.find(" ");
        }
        //Step 3. Extract the operators and separators from Medium.cc.
        for (std::string separator : separators){
            int index = line.find(separator);
            while (index != std::string::npos){
                components.push_back(line.substr(0, index));
                components.push_back(separator);
                line = line.erase(index);
                index = line.find(separator);
            }
        }
        for (std::string operator : operators){
            int index = line.find(operator);
            while (index != std::string::npos){
                components.push_back(line.substr(0, index));
                components.push_back(operator);
                line = line.erase(index);
                index = line.find(operator);
            }
        }
        return components;
    }

    list<Lexeme> parseComponents(list<string> components){
        list<Lexeme> lexemes;
        for (std::string component : components){
            for (std::string keyword : keywords){
                if (component == keyword){
                    lexemes.push_back({Keyword, component});
                    break;
                }
            }
        }
    }

};