/* Mozilla Public License, v2 | Vladyslav Korol
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
        Keyword, Operator, Separator, Number, String, Boolean, Unknown,
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
        return line;
    }

    /// @brief Splits the line into a linked list of individual
    /// meaningful parts that are ready to be parsed as lexemes.
    std::list<std::string> splitIntoComponents(std::string line){
        std::list<std::string> components; //TO-DO: implement multiline string 
        //Step 1. Extract the strings.    //with triple quotations detection.
        int quote = line.find("\"");
        while (quote != std::string::npos){
            int end = line.find("\"", quote + 1);
            if (end == std::string::npos) break;
            components.push_back(line.substr(quote, end - quote + 1));
            line = line.substr(end + 1);
            quote = line.find("\"");
        }
        //Step 2. Remove the whitespaces.
        int whitespace = line.find(" ");
        while (whitespace != std::string::npos){
            components.push_back(line.substr(0, whitespace));
            line = line.substr(whitespace + 1);
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
        // operator is a C++ keyword, therefore we should 
        // prefix it with an underscore to avoid ambiguity. 
        for (auto _operator : operators){
            int index = line.find(_operator);
            while (index != std::string::npos){
                components.push_back(line.substr(0, index));
                components.push_back(_operator);
                line = line.erase(index);
                index = line.find(_operator);
            }
        }
        return components;
    }

    //The string-to-lexeme identifier table to keep track of all user definitions.
    std::map<std::string, Lexeme> identifiers;
    /// @brief Parses the extracted textual units into individual highly descriptive lexemes.
    /// @param components The list of string representations of lexemes in the code.
    /// @param flags An array of tokens to use to identify new identifier declarations.
    /// @return The array of matched lexemes.
    std::list<Lexeme> parseComponents(std::list<std::string> components, Token* flags){
        int count; //The index of the flag to refer to.
        bool found; //The variable to identify if a lexeme was matched in the first phase.
        std::list<Lexeme> lexemes;
        // Searching through constant lexemes.
        for (std::string component : components){
            for (std::string keyword : keywords){
                if (component == keyword){
                    lexemes.push_back({Keyword, component});
                    found = true; break;
                }
            }
            if (found) continue; //We verify if component was matched to not keep
                             //going through the rest of lexemes when it was found.
            for (std::string _operator : operators){
                if (component == _operator){
                    lexemes.push_back({Operator, component});
                    found = true; break;
                }
            }
            if (found) continue;
            for (std::string separator : separators){
                if (component == separator){
                    lexemes.push_back({Separator, component});
                    found = true; break;
                }
            }
            if (!found){ //If component was not found, it means it's an identifier.
                //If the identifier was found in the table:
                if (identifiers.find(component) != identifiers.end()){
                    lexemes.push_back({flags[count], component});
                    count++;
                } else {
                    //TO-DO: implement valid-invalid identifier verification.
                    lexemes.push_back({flags[count], component});
                    count++;
                }
            }
        }
        
        return lexemes;
    }

};