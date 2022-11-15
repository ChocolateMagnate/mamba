#include <map>
#include <string>
#include <vector>
namespace mamba{

    //The string-to-lexeme identifier table to keep track of all user definitions.
    auto *identifiers = new std::map<std::string, Lexeme>();

   /* class PyClass{
        public:
            std::vector<int> properties, methods;
            int operator[](std::pair<std::string, std::string> key){
                if (std::get<0>(key) == "Properties"){}
                else if (std::get<0>(key) == "Methods"){}
        }
    };*/
};
