#include "/workspaces/python-interpreter/Source/Compiler/Lexer.cc"
#include <string>
#include <list>
#include <iostream>
#define print(x) for (auto item : x) std::cout << item.first << "\n"
int main(){
    auto lexemes2 = mamba::buildLexemes("print(\"Hello !, \" + \"World !, \")");
    //print(lexemes2);
    for (auto lexeme : lexemes2){
        std::cout << lexeme.first << " " <<  "\n";
    }
    std::cout << "Done!\n";
    return 0;
}