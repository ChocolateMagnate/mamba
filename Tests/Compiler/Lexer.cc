#include "/workspaces/python-interpreter/Source/Compiler/Lexer.cc"
#include <string>
#include <list>
#include <iostream>
#define print(x) for (auto item : x) std::cout << item << "\n"
int main(){
    std::string code = "a = 5\nprint(\"Hello World!\" + \"We are from Ukraine!\")";
    std::vector<std::string> delimiters = {"[", "]", "{", "}", "(", ")", "=", ",", ":", ";", "+"};
    auto lexemes2 = mamba::breakDown(code);
    print(lexemes2);


    return 0;
}