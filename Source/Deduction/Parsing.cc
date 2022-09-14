/* Mozilla Public License, v2 | Vladyslav Korol
 * This files takes the stream of statement tokens and
 * parses them into a grammar tree describing the logic
 * of the program. At the end, it generates the Python
 * Bytecode (.pyc) file for later faster execution. */
#include <iostream>
#include <string>

namespace Interpreter {
    /* Grammar parsing is the second stage of
     * interpretation that builds the syntax 
     * tree responsible for representing the 
     * ideas put into the code in machine-readable 
     * form. Then it is cached as .pyc files 
     * to reuse the initial analysis. later.*/
    /// @brief Conveys the types of legal statements.
    enum Statement {
        ClassDeclaration, FunctionDeclaration, Lambda, ModuleImport, 
        Assignment, Modification, Returning, Match, CaseBranch,
        IfBranch, ElifBranch, ElseBranch, ForLoop, WhileLoop,
        Break, Continue, Pass, Delete, Raise, Try, Except, Finally,
        With, Yield, Assert 
    };

};