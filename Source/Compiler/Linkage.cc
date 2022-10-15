/* Mozilla Public License, v2 | Vladyslav Korol
 * This file accesses parses the linked dependencies 
 * and registers them into the identifier table. */
#include <iostream>
#include <fstream>

namespace Interpreter {
    /* Linkage is an essential step required in any programming
     * language that takes care of searching for the items in
     * the dependencies and registering them to the identifier
     * table that can be used in the source code. Additionally 
     * to that, the linker stamps their locations and calls 
     * the dependent items once they are used within the code.*/
    /// @brief Searches the dependent items in the Python library files and
    /// adds them to the identifier table. Raises an exception if failed.
    /// @param dependency The library to search.
    void linkPythonDependency(std::string dependency, std::string* items){
        
    }

    /// @brief Imports all the files from a Python library into the identifier table.
    void linkPythonEverything(std::string dependency){

    }
    /// @brief Links outer dependencies commonly written in compiled languages for performance boost.
    void linkCompiledDependency(std::string dependency, std::string* items){

    }

    /// @brief Imports all items from a compiled dependency.
    void linkCompiledEverything(std::string dependency){

    }
};