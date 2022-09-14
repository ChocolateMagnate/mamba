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
};