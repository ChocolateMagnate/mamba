#include "/workspaces/python-interpreter/Source/Compiler/Lexer.cc"
#include <string>
#include <list>
#include <iostream>
#define print(x) for (auto item : x) std::cout << item.first << "\n"
int main(){
    std::string code = "print(\"Hello World!\" + \"We are from Ukraine!\") #Hello world program";
    std::string commentless = mamba::clearComments(code).first;
    std::cout << commentless << "\n";
    auto components = mamba::extractStrings(commentless);
    std::cout << "-----------------" << std::endl;
    auto lexemes = mamba::buildLexemes(components);
    std::cout << "-----------------" << std::endl;
    return 0;
}