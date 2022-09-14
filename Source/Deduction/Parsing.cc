/* Mozilla Public License, v2 | Vladyslav Korol
 * This files takes the stream of statement tokens and
 * parses them into a grammar tree describing the logic
 * of the program. At the end, it generates the Python
 * Bytecode (.pyc) file for later faster execution. */
#include <iostream>
#include <string>
#include "Commons.cc"
namespace Interpreter {
    /* Grammar parsing is the second stage of
     * interpretation that builds the syntax 
     * tree responsible for representing the 
     * ideas put into the code in machine-readable 
     * form. Then it is cached as .pyc files 
     * to reuse the initial analysis later.*/
    /// @brief Conveys the types of legal statements.
    enum StatementType {
        ClassDeclaration, FunctionDeclaration, Lambda, ModuleImport, 
        Assignment, DataModification, Returning, Match, CaseBranch,
        IfBranch, ElifBranch, ElseBranch, ForLoop, WhileLoop, Break,
        Continue, Pass, Delete, Raise, Try, Except, Finally, With,
        Yield, Assert 
    };
    /// @brief Covers all grammatical relationships between the lexemes.
    enum GrammarRelation{
        Assign, Add, Subtract, Multiply, Divide, AddTo, SubtractFrom, MultiplyBy, DivideBy, Jump,
        CompareEquals, CompareLess, CompareMore, CompareNotEquals, CompareNotMore, CompareNotLess,
        BooleanAnd, BooleanOr, BooleanNot, Repeat, Break, Continue, DeclareClass, DeclareObject,
        DeclareFunction, CallFunction, ThrowException, CatchException
    };
    
    typedef struct GrammarLexeme{
        Lexeme lexeme;

    } GrammarLexeme;
};