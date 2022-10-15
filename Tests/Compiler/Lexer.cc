#include "/workspaces/python-interpreter/Source/Compiler/Lexer.cc"
#include <string>
#define print(x) std::cout << x << "\n"
int main(){
    std::string code = "print(\"Hello World!\") #Hello world program";
    auto commentless = mamba::clearComments(code);
    auto components = mamba::splitIntoComponents(commentless.first);
    std::cout << code << std::endl;
    //std::cout << commentless.first << std::endl;
    for (auto component : components) std::cout << component << std::endl;
    std::cout << components.size() << std::endl;
    return 0;
}