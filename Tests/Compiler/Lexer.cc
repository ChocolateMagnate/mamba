#include "/workspaces/python-interpreter/Source/Compiler/Lexer.cc"
#include <string>
int main(){
    std::string code = "print(\"Hello World!\") #Hello world program";
    auto commentless = mamba::clearComments(code);
    auto components = mamba::parseComponents(mamba::splitIntoComponents(commentless.first));
    //std::cout << commentless.first << std::endl;
    for (auto component : components) std::cout << component.label << std::endl;
    return 0;
}