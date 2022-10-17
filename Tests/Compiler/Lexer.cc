#include "/workspaces/python-interpreter/Source/Compiler/Lexer.cc"
#include <string>
#include <list>
#include <iostream>
#define print(x) for (auto item : x) std::cout << item.first << item.second << "\n"
int main(){
    std::string code = "print(\"Hello World!\") #Hello world program";
    std::list<std::string> components = {"print", "(", "\"Hello ",  "World!\"", ")", " ", "#Hello world program"};
    for (auto x : mamba::extractStrings(&components))
        std::cout << x.first << " " << x.second << "\n";
    std::cout << "-------------------\n";
    return 0;
}