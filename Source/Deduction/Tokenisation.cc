/* Mozilla Public License, v2 | Vladyslav Korol
 * This file divides the Python source code into a table
 * of lexemes: keywords, ids, etc. for further parsing.*/
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include "./Source/Commons.cc"
namespace Interpreter {
    /* Tokenisation is always the first step done
     * in any programming language. It deals with
     * removing comments and extracting different
     * tokens from the code such as keywords, 
     * operators, separators, and user-defined 
     * identifiers. Once the code is presented as
     * steam of lexemes, it can be grammatically 
     * matched into a syntax tree and be executed.*/

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
        int quote = line.find('"');
        while (quote != std::string::npos){
            int end = line.find('"', quote + 1);
            if (end == std::string::npos) break;
            components.push_back(line.substr(quote, end - quote + 1));
            line = line.substr(end + 1);
            quote = line.find('"');
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
        for (std::string _operator : operators){
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

    /// @brief Parses the extracted textual units into individual highly descriptive lexemes.
    /// @param components The list of string representations of lexemes in the code.
    /// @return The array of matched lexemes.
    std::list<Lexeme> parseComponents(std::list<std::string> components){
        //TO-DO: add details to the lexemes;
        int count;           //The index of the unknown lexeme to refer to.
        bool found = false; //The variable to identify if a lexeme was matched to escape the loop.
        bool searchEquationMark = false; //The flag if we should look for an equation mark to validate an unknown lexeme.
        std::string previous; //Keeping track of the previous token for class and function declarations.
        std::vector<Lexeme*> unknowns; //The record of unknown identifiers.
        std::list<Lexeme> lexemes;
        for (std::string component : components){
            //Firstly we need to verify if there's an equation mark 
            //after an unknown id to assign it as a local object.
            if (component == " " || component == ";") //If the statement is terminated, 
                searchEquationMark = false;          //an unknown lexeme cannot be assigned.
            if (searchEquationMark){
                if (component == "=") 
                    (unknowns[count - 1])->type = Object;
            }
            //Then we search through the constant language-defined lexemes.
            for (std::string _operator : operators){
                 //operator is a C++ keyword, therefore we need
                //to prefix it with an underscore to avoid ambiguity.
                if (component == _operator){
                    lexemes.push_back({Operator, component});
                    found = true; break;
                }
            }
            if (found) continue; //We verify if component was matched to not keep
                                //going through the rest of lexemes when it was found.
            for (std::string keyword : keywords){
                if (component == keyword){
                    lexemes.push_back({Keyword, component});
                    found = true; break;
                }
            }
            if (found) continue;
            for (std::string separator : separators){
                //Separators are the rarest symbols in the code, hence they should 
                //stay at the end to find other more frequent lexemes faster.
                if (component == separator){
                    lexemes.push_back({Separator, component});
                    found = true; break;
                }
            }
            //If component was not found, it means it's an identifier.
            if (!found){ 
                //If the identifier was found in the table:
                auto lookup = identifiers->find(component);
                if (lookup != identifiers->end()){
                    //Thanks for www.geeksforgeeks.com/iterators-c-stl for the tutorial.
                    lexemes.push_back({lookup->second.type, component});
                    count++;
                } else {
                    /* If id is unknown, we can verify if it is a name of a class or a 
                     * function by looking at the previous keyword, it should be class
                     * or def. If it is a variable, it should have an equation mark after
                     * it. We will verify if the name declares a class or a function with
                     * previous variable and add it as an unknown token if it wasn't valid,
                     * and then if we detect an applicable equation mark, we will edit it.*/
                    if (previous == "class") lexemes.push_back({Class, component});
                    else if (previous == "def") lexemes.push_back({Function, component});
                    else {
                        lexemes.push_back({Unknown, component});
                        unknowns.push_back(&(*lexemes.end()));
                        searchEquationMark = true;
                        count++;
                    }
                }
            }
            previous = component;
        }
        return lexemes;
    }
};