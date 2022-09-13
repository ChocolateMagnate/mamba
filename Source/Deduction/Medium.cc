/* GNU Lesser General Public License | Valdyslav Korol
 * This file specifies and implements the intermediate 
 * representation (IR) used to convey the source code.*/
#include <iostream>
#include <string>

namespace Interpreter {
    /* Intermediate representation is an optional step
     * but compilation naturally developed into favouring
     * it. IR allows modular portability, and in case of 
     * interpretation it allows to save time by avoiding
     * parsing the code again but reusing the result and
     * modifying it when changes happen in the code.*/
    /// @brief The IR used by the interpreter. It represents the 
    /// grammar tree as a sequence of objects responsible for
    /// individual lines, thus allowing greater flexibility.
    class Bytecode{

    };
};