/* Mozilla Public License, v2 | Vladyslav Korol
 * This file divides the Python source code into a table
 * of lexemes: keywords, ids, etc. for further parsing.*/
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
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
    /// @brief Extracts the string as a whole lexeme once a quote is found.
    /// @param elements The linked list of components to search in.
    list<tuple<string, Token, bool>> extractStrings(string line, bool closedLine = true){
        list<tuple<string, Token, bool>> components;
        int start = 0, end = 0;
        if (!closedLine){
            end = line.find("\"\"\"");
            if (end == string::npos){
                end = line.find("\"");
                if (end == string::npos) return list(tuple(line, Token::String, false));
                else
                    components.append(tuple(line.substring(0, end), Token::String, true));
            }
        }
        // Step 1. Extract all triple strings.
        start = line.find("\"\"\"");
        while (start != string::npos){
            end = source.find("\"\"\"", start + 3);
            if (end == string::npos){
                components.append({line.substring(start), Token::String, false});
                break;
            }
            components.append({line.substring(start, end + 3), Token::String, true});
            start = line.find("\"\"\"", end + 3);
        }
        // Step 2. Extract all single strings.
        start = line.find("\"");
        while (start != string::npos){
            end = line.find("\"", start + 1);
            if (end == string::npos){
                components.append({line.substring(start), Token::String, false});
                break;
            }
            components.append({line.substring(start, end + 1), Token::String, true});
            start = line.find("\"", end + 1);
        }
        return components;
    }
    /// @brief Splits the line into a linked list of individual
    /// meaningful parts that are ready to be parsed as lexemes.
    list<string> splitIntoComponents(string line){
        list<string> components; //TO-DO: implement multiline string 
                                //with triple quotations detection.
        list<string>* address = &components;
        cout << "Components initialised.\n";
        //Step 0. Extract the indentations.
        for (string indentation : mamba::indentations){
            int index = line.find(indentation);
            while (index != string::npos){
                indentationCounter++;
                line = line.replace(index, indentation.length(), "");
                line = line.substring(index + 4);
            }
        }
        //Step 1. Remove the whitespaces.
        int whitespace = line.find(" ");
        while (whitespace != string::npos){
            int quotation = line.find('"');
            components.append(line.substring(0, whitespace));
            line = line.substring(whitespace + 1);
            whitespace = line.find(" ");
        }
        cout << "Whitespaces removed.\n";
        //Step 2. Extract the strings.
        int start = line.find('"');
        while (start != string::npos){
            int end = line.find('"', start + 1);
            extractStrings(address);
            line = line.substring(start + 1) + line.substring(end + 1);
            start = line.find('"');
        }
        cout << "Strings extracted.\n";  
        //Step 3. Extract the keywords and other symbols from Commons.cc.
        bool found = false;
        list<string>::iterator it = components.begin();
        for (string component : components){
            for (string keyword : mamba::keywords){
                if (component.find(keyword) != string::npos){
                    components.insert(it, keyword);
                    found = true;
                    break;
                }
            }
            if (found) break;
            for (string _operator : mamba::operators){
                if (component.find(_operator) != string::npos){
                    components.insert(it, _operator);
                    found = true;
                    break;
                }
            }
            if (found) break;
            for (string separator : mamba::separators){
                if (component.find(separator) != string::npos){
                    components.insert(it, separator);
                    break;
                }
            }
            it++;
            found = false;
        }

        cout << "Keywords, operators and separators extracted.\n";
        return components;
    }

    /// @brief Parses the extracted textual units into individual highly descriptive lexemes.
    /// @param components The list of string representations of lexemes in the code.
    /// @return The array of matched lexemes.
    auto parseComponents(list<string> components){
        //TO-DO: add details to the lexemes;
        int count;           //The index of the unknown lexeme to refer to.
        bool found = false; //The variable to identify if a lexeme was matched to escape the loop.
        bool searchEquationMark = false; //The flag if we should look for an equation mark to validate an unknown lexeme.
        string previous; //Keeping track of the previous token for class and function declarations.
        vector<Lexeme*> unknowns; //The record of unknown identifiers.
        list<Lexeme> lexemes;
        for (string component : components){
            //Firstly we need to verify if there's an equation mark 
            //after an unknown id to assign it as a local object.
            if (component == " " || component == ";") //If the statement is terminated, 
                searchEquationMark = false;          //an unknown lexeme cannot be assigned.
            if (searchEquationMark){
                if (component == "=") 
                    (unknowns[count - 1])->type = Object;
            }
            //Then we search through the constant language-defined lexemes.
            for (string _operator : operators){
                 //operator is a C++ keyword, therefore we need
                //to prefix it with an underscore to avoid ambiguity.
                if (component == _operator){
                    lexemes.append({Operator, component});
                    found = true; break;
                }
            }
            if (found) continue; //We verify if component was matched to not keep
                                //going through the rest of lexemes when it was found.
            for (string keyword : keywords){
                if (component == keyword){
                    lexemes.append({Keyword, component});
                    found = true; break;
                }
            }
            if (found) continue;
            for (string separator : separators){
                //Separators are the rarest symbols in the code, hence they should 
                //stay at the end to find other more frequent lexemes faster.
                if (component == separator){
                    lexemes.append({Separator, component});
                    found = true; break;
                }
            }
            //If component was not found, it means it's an identifier.
            if (!found){ 
                //If the identifier was found in the table:
                auto lookup = identifiers->find(component);
                if (lookup != identifiers->end()){
                    //Thanks for www.geeksforgeeks.com/iterators-c-stl for the tutorial.
                    lexemes.append({lookup->second.type, component});
                    count++;
                } else {
                    /* If id is unknown, we can verify if it is a name of a class or a 
                     * function by looking at the previous keyword, it should be class
                     * or def. If it is a variable, it should have an equation mark after
                     * it. We will verify if the name declares a class or a function with
                     * previous variable and add it as an unknown token if it wasn't valid,
                     * and then if we detect an applicable equation mark, we will edit it.*/
                    if (previous == "class") lexemes.append({Class, component});
                    else if (previous == "def") lexemes.append({Function, component});
                    else {
                        lexemes.append({Unknown, component});
                        unknowns.append(&(*lexemes.end()));
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