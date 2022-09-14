/* Mozilla Public License, v2 | Vladyslav Korol
 * This file validates the semantics of the source code
 * and makes sure it correlates to the language's rules.*/
#include <iostream>
#include <string>
#include <vector>
#include <map>
namespace Interpreter{
    /* Semantic analysis is the third and final step
     * in the source analysis. It deals with verifying
     * the code makes sense to the language specification
     * and detects errors like type inconsistency, using
     * undeclared variables, flow control validation
     * and misusing the reserved lexemes. Even though 
     * Python is dynamically-typed, the interpreter covers
     * edge cases inspired by github.com/google/pytype. */
     
     void raiseArithmeticError(std::string line){
        throw line;
     }

     void raiseAssertionError(std::string line){
        throw line;
     }

     void raiseAttributeError(std::string line){
        throw line;
     }

     void raiseEOFEError(std::string line){
        throw line;
     }
     void raiseFloatingPointError(std::string line){
        throw line;
     }
     void raiseGeneratorExit(std::string line){
        throw line;
     }
     void raiseImportError(std::string line){
        throw line;
     }
     void raiseIndentationError(std::string line){
        throw line;
     }
     void raiseIndexError(std::string line){
        throw line;
     }
    void raiseKeyError(std::string line){
        throw line;
    }
    void raiseKeyboardInterrupt(std::string line){
        throw line;
    }
    void raiseLookupError(std::string line){
        throw line;
    }
    void raiseMemoryError(std::string line){
        throw line;
    }
    void raiseNameError(std::string line){
        throw line;
    }
    void raiseNotImplementedError(std::string line){
        throw line;
    }
    void raiseOSError(std::string line){
        throw line;
    }
    void raiseOverflowError(std::string line){
        throw line;
    }
    void raiseReferenceError(std::string line){
        throw line;
    }
    void raiseRuntimeError(std::string line){
        throw line;
    }
    void raiseStopIteration(std::string line){
        throw line;
    }
    void raiseSyntaxError(std::string line){
        throw line;
    }
    void raiseTabError(std::string line){
        throw line;
    }
    void raiseSystemError(std::string line){
        throw line;
    }
    void raiseSystemExit(std::string line){
        throw line;
    }
    void raiseTypeError(std::string line){
        throw line;
    }
    void raiseUnboundLocalError(std::string line){
        throw line;
    }
    void raiseUnicodeError(std::string line){
        throw line;
    }
    void raiseUnicodeEncodeError(std::string line){
        throw line;
    }
    void raiseUnicodeDecodeError(std::string line){
        throw line;
    }
    void raiseUnicodeTranslateError(std::string line){
        throw line;
    }
    void raiseValueError(std::string line){
        throw line;
    }
    void raiseZeroDivisionError(std::string line){
        throw line;
    }
};