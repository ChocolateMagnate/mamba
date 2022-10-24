#include "/workspaces/python-interpreter/Source/Compiler/Lexer.cc"
#include <string>
#include <list>
#include <iostream>
#define print(x) for (auto item : x) std::cout << item << "\n"
int main(){
    std::string code = "print(\"Hello World!\" + \"We are from Ukraine!\")";
    /*std::string code2 = "import tensorflow as tf", code3 = "ls = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]";
    std::string commentless = mamba::clearComments(code).first;
    std::cout << commentless << "\n";
    auto components = mamba::extractStrings(commentless);
    std::cout << "-----------------" << std::endl;
    auto lexemes = mamba::buildLexemes(components);
    std::cout << "-----------------" << std::endl;
    mamba::buildLexemes(mamba::extractStrings(mamba::clearComments(code2).first));
    auto result = mamba::extractStrings(mamba::clearComments(code3).first);*/
    std::vector<std::string> delimiters = {"[", "]", "{", "}", "(", ")", ",", ":", ";", "+"};
    auto lexemes2 = mamba::divideBy(code, delimiters);
    print(lexemes2);


    return 0;
}