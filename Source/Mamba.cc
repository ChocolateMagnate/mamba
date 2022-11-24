/* Mamba Python by Vladyslav Korol | Apache License 2.0
 * The core of the runtime enviornment that takes the source 
 * input and makes the series of external calls in the
 * namespace to execute the code. The app works in CLI. */
#include <iostream>
#include <fstream>
#include "CLI.cc"
#include "Compiler/Compiler.hh"
#include "Interpreter/Interpreter.hh"
#include "Standard/Standard.hh"
namespace mamba {
    /* The heart of the compiler. It is built as
     * a command line interface tool and unites all 
     * components into read-execute-repeat loop.*/
    
    enum Instruction{
        Import, Initialise, Reassign, 
    };
    /// @brief Interprets the input line into machine-readable IR. 
    /// @return The array of integers to pass to the execute function.
    std::vector<int> interpret(std::string line) {

    }
    /// @brief Executes a single line of code in REP loop.
    /// @param query The pointer to the array of integers to be matched.
    void execute(std::vector<int> query) {
        for (int command : query){
            switch (command){}
        }
    }

    void run(const char* file){
        std::ifstream source(file);
        std::string line;
        while (std::getline(source, line)){
            execute(interpret(line));
        }
    }

    /// @brief Inspects the source file. If path alone is passed, 
    /// Mamba profiles the code and prints the results. If options 
    /// are passed, Mamba executes the code with the given options.
    /// @param path The path to the source file as a C-string.
    void inspect(const char* path){
        std::fstream source(path);

    }

    /// @brief Generates the configuration mamba-settings.json file
    /// and links it to the settings in the speicifed directory.
    void set(const char* path){

    }

    /// @brief Unlinks the specified configuration file.
    void unset(const char* path){

    }
    void build();

    void version() {
        std::cout << "Mamba Python 1.0.0\n";
        //To add version details and contributors.
    }

    void license() {
        std::ifstream license("LICENSE");
        if (license.is_open()) std::cout << license.rdbuf();
        else std::cout << "Apache License v. 2.\n";
    }

    void about() {
        std::cout << "Mamba Python is (yet another) implementation " <<
            "of the Python programming language.\n It was originally " <<
            "supposed to be a regular Python interpreter, but later " <<
            "the author was sparkled by an idea to proceed it into a " <<
            "full-fledged Just-in-Time compiled runtime enviornment.\n" <<
            "The project is aimed to offer Python developers and ecosystem " <<
            "to ship and distribute the code without the need to install " <<
            "the interpreter but relies on the runtime enviornment instead, " <<
            "similarly to Java and .NET. On top of that, it offers complete" <<
            "backward compatibility with the Python language and all C extensions.\n" << 
            "The project not only improves the performance speed of any written " <<
            "application, but also offers compitability with C and C++ libraries." << std::endl;
    }
};

int main(int argc, char** argv) {
    if (argc == 1)
        std::cout << "Welcome to Mamba Python v1.0.0.\n" <<
            "Type mamba [PATH] to build a Python app.\n" <<
            "Type mamba -h to get help." << std::endl;
    else if (argv[1] == "run") mamba::run(argv[2]);
    else if (argv[1] == "inspect") mamba::inspect(argv[2]);
    else if (argv[1] == "set") mamba::set(argv[2]);
    else if (argv[1] == "unset") mamba::unset(argv[2]);
    else if (argv[1] == "build") mamba::build();
    else if (argv[1] == "-h") mamba::help();
    else if (argv[1] == "-v") mamba::version();
    else if (argv[1] == "-l") mamba::license();
    else if (argv[1] == "-a") mamba::about();
    else std::cout << "Unknown command. Type mamba [PATH] to build an application " 
            << "or mamba -h to get help." << std::endl;
    return 0;
}
