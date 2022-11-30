/* Mamba Python by VLadyslav Korol | Apache License 2
 * This file declares the built-in Pythonic functions 
 * that are impoted automatically and are ready to be used. 
 * Source: docs.python.org/3/library/functions.html */

#pragma once
#include <fstream>
#include "./Interpreter/Base.hh"
#include "./Compiler/Parser.hh"
namespace mamba {
    /// @brief Returns the absolute value of the number.
    unsigned int abs(mamba::Bitset x);
    /// @brief Returns the asynchrnous iterator from the iterable.
    mamba::Bitset aiter(mamba::Bitset asynchIterable);
    /// @brief Returns True if all elements of the iterable are true or the iterable is empty.
    bool all(mamba::Bitset iterable);
    /// @brief Returns the next item from the asynchronous iterator.
    mamba::Bitset* anext(mamba::Bitset asynchIterator);
    /// @brief Returns True if any element of the iterable is true.
    /// If the iterable is empty, returns False.
    bool any(mamba::Bitset iterable);
    /// @brief Returns the ASCII representation of the object.
    const char* ascii(mamba::Bitset object);
    /// @brief Returns the binary representation of the object.
    /// @note If the passed value is not an intefer, it will call __int__.
    const char* bin(mamba::Bitset number);
    /// @brief Drops the program into the debgger mode.
    void breakpoint(mamba::params args);
    /// @brief Returns true if the object is callable (operloads the () operator).
    bool callable(mamba::Bitset object);
    /// @brief Returns the character whose unicode is the codepoint.
    char chr(int codepoint);
    /// @brief Compiles the source code into abstract syntax tree that can be executed with
    /// exec() or eval(). Raises all sorts of errors if the code is invalid.
    /// @param source The source string to be compiled.
    /// @param filename The name of the file from which the source was loaded.
    /// @param mode The mode of compilation: exec, eval or single.
    /// @param flags 
    /// @param dont_inherit 
    /// @param optimize The indicator of the optimization level.
    /// @note If the source contains only expressions, it must contain the newline terminator.
    /// @return Abstract syntax tree of the compiled source.
    mamba::AbstractSyntaxTree compile(mamba::Bitset source, mamba::String filename,
        mamba::String mode, int flags = 0, bool dont_inherit = false, int optimize  = -1);

    /// @brief Deletes an attribute (method or field) from the object.
    /// @param object The object to modify.
    /// @param name The name of the property to remove.
    void delattr(mamba::Bitset object, mamba::String name);
    /// @brief Lists all the files in the local directory.
    mamba::List dir();
    /// @brief Lists all the properties of the object. If it
    /// implements __dir__ method, it will be called.
    /// @param object The object to analyse.
    /// @return The list of properties.
    mamba::List dir(mamba::Bitset object);
    /// @brief For the non-complex numbers x and y, returns the pair of
    /// their quotient and the remainder
    std::pair<int, int> divmod(int x, int y);
    /// @brief Yields the pair of the index and next element of the iterable.
    /// @param iterable The iterable to iterate over.
    /// @param start The index to start from.
    /// @return The generator that will yield the pairs on demand.
    mamba::Generator enumerate(mamba::Bitset iterable, int start = 0);
    /// @brief Evaluates the expression and returns the result. No statements are allowed.
    /// @param expression The expression to evaluate.
    /// @param globals An optional dictionary of global variables.
    /// @param locals An optional dictionary of local variables.
    /// @return The result of the expression. For example, eval("a += 5", {"a": 3}) will return 8.
    mamba::Bitset eval(mamba::Bitset expression, mamba::Dictionary* globals
                 = nullptr, mamba::Dictionary* locals = nullptr);
    /// @brief Executes dynamically-configured source code from a string. Beware the security risks.
    /// @param source The string containing the source code.
    /// @param globals An optional dictionary of global variables.
    /// @param locals An optional dictionary of local variables.
    void exec(mamba::String& source, mamba::Dictionary* globals = nullptr,
              mamba::Dictionary* locals = nullptr);
    /// @brief Executes dynamically-configured source from the AST. Beware the security risks.
    /// @param ast The abstract syntax tree to execute.
    /// @param globals An optional dictionary of global variables.
    /// @param locals An optional dictionary of local variables.
    void exec(mamba::AbstractSyntaxTree* ast, mamba::Dictionary* globals = nullptr,
              mamba::Dictionary* locals = nullptr);
    /// @brief RFilters the iterable by the conditions.
    /// @param filterFunction The function that should return true if
    /// the element should be included.
    /// @param iterable The iterable to filter.
    /// @return The filtered iterable with the elements that passed the filter.
    mamba::Bitset filter(mamba::Function* filterFunction, mamba::Bitset& iterable);
    /// @brief Returns whatever is the assigned to the named property of the object.
    mamba::Bytecode getattr(mamba::Bitset& object, mamba::String& name);
    /// @brief Returns the dictionary containing the global variables in the file scope.
    mamba::Dictionary* globals();
    /// @brief Indicates whether the object posseses the specified property. 
    bool hasattr(mamba::Bitset& object, mamba::String& name);
    /// @brief Returns the hashed value of the object as an unsigned integer.
    unsigned int hash(mamba::Bitset& object);
    /// @brief Casts the integer representation of the object into the hexadecimal string.
    /// @param number An integer or otherwise bitset that implements __int__ to cast.
    /// @return C-string containing the hexadecimal representation of the number.
    const char* hex(mamba::Bitset& number);
    /// @brief Returns the unique integral identity of the object.
    /// @note Two objects with non-overlapping lifetimes may have the same id().
    int id(mamba::Bitset* object);
    /// @brief Suspects the program execution and awaits the user input.
    /// @param prompt The text to display in the terminal.
    /// @return The Mamba string containing the user input. It needs manual casting if needed.
    mamba::String input(mamba::String prompt = "");
    /// @brief Compares if the object is an instance of the class. This function can
    /// traverse the inheritance tree and tell if the object belongs to a distant parent.
    bool isinstance(mamba::Bitset& object, mamba::Type& type);
    /// @brief Indicates if the given class is a subclass (direct, indirect, virtual)
    /// of the description.
    /// @param type The class to compare.
    /// @param classinfo The tuple of properties class must satisfy.
    /// @return True if class satisfies the subclass critera, False otherwise.
    bool issubclass(mamba::Type& type, mamba::Type& classinfo);
    /// @brief Generates an iterator from the iterable object.
    /// @param iterable The object that should support iteration
    /// (implement __iter__ or __getitem__). Otherwise, ValueError is raised.
    /// @return The generator that will yield the elements on demand.
    mamba::Generator iter(mamba::Bitset& iterable);
    /// @brief Generates an iterator from the iterable function.
    /// @param iterable The function that will yield the values.
    /// @param sentinel The value that will stop the iteration.
    /// @return The generator that will yield the elements on demand.
    mamba::Generator iter(mamba::Function& iterable, mamba::Bitset& sentinel);
    /// @brief Returns the length of the object. If not provided, calls __len__.
    unsigned int len(mamba::Bitset& object);
    /// @brief Returns the dictionary containing the local variables in the current scope.
    mamba::Dictionary* locals();
    /// @brief Generates an iterator that runs the mapFunction on each element of the iterable.
    mamba::Bitset map(mamba::Function& mapFunction, mamba::Bitset& iterable);
    /// @brief Returns the maximum value in the iterable.
    mamba::Bitset max(mamba::Bitset& iterable);
    /// @brief Returns the maximum value from the parameters.
    mamba::Bitset max(mamba::params& args);
    /// @brief Returns the minimal value in the iterable.
    mamba::Bitset min(mamba::Bitset& iterable);
    /// @brief Returns the minimal value from the parameters.
    mamba::Bitset min(mamba::params& args);
    /// @brief Yields the next element of the iterable.
    /// @param generator The generator to iterate over.
    /// @return The value of the next element.
    mamba::Bitset next(mamba::Generator& generator);
    /// @brief Yields the next element of the iterable.
    /// @param generator The generator to iterate over.
    /// @param default The value to return if the generator is exhausted.
    /// @return The value of the next element or the default value.
    mamba::Bitset next(mamba::Generator& generator, mamba::Bitset& default);
    /// @brief Casts the integer representation of the object into the octal string.
    /// @param number An integer or otherwise bitset that implements __int__ to cast.
    /// @return The octal representation of the number.
    const char* oct(mamba::Bitset& number);
    /// @brief Opens the file.
    /// @param filename The location of the file.
    /// @param mode The mode how to treat the file: read, write, append, binary, etc.
    /// @return Standard managed fstream object.
    std::fstream open(mamba::String& filename, mamba::String& mode = "r");
    /// @brief Casts the character by the unicode encoding. The inverse of chr().
    /// @param character The character to cast.
    /// @return The unsigned integer representation according to the unicode.
    unsigned int ord(char character);
    /// @brief Raises the base to the power of the exponent.
    /// @param base The basic number to raise.
    /// @param exponent The degree to raise the base to.
    /// @param modulo An optional modulo to apply to the result.
    /// @note If the modulo is provided, the result is the remainder of the division.
    /// @return The result of the operation.
    long long int pow(mamba::Bitset& base, mamba::Bitset& exponent, mamba::Bitset& modulo = 0);
    /// @brief Prints the object to the standard output.
    /// @param text The object (integer, string, custom object) to print.
    /// @param separator If multiple objects are provided, this is the separator.
    /// @param end The character to print at the end of the function.
    /// @param flush The flag to indicate whether to flush the output (std::endl).
    void print(mamba::Bitset** text, const char* separator = " ",
                const char* end = "\n", bool flush = false);
    
    /// @brief Reverses the iterable.
    /// @param iterable The object to reverse.
    /// @return The reversed iterator.
    mamba::Bitset reversed(mamba::Bitset& iterable);
    /// @brief Rounds the number to the specified number of digits.
    /// @param number The number to round.
    /// @param precision The number of digits to round to.
    /// @return The rounded number.
    double round(mamba::Bitset& number, unsigned int precision = 0);
    /// @brief Assigns value to the object property.
    void setattr(mamba::Bitset& object, mamba::String& name, mamba::Bytecode& value);
    /// @brief Returns the new sorted iterable.
    mamba::Bitset sorted(mamba::Bitset& iterable);
    /// @brief Sums all the elements of the iterable. Raises ValueError if the iterable
    /// contains values that do not overload __add__.
    /// @param iterable The iterator to sum.
    /// @param start The initial value to start the sum with.
    /// @return The sum of all the elements.
    unsigned long long int sum(mamba::Bitset& iterable, unsigned int start = 0);
    /// @brief Returns the __dict__ property of any object.
    mamba::Bitset vars(mamba::Bitset& object);
    /// @brief Patches the values into a sequence of tuples.
    /// @param first The first iterable to zip.
    /// @param second The second iterable to append.
    /// @param strict If true, the function will raise ValueError
    /// if the iterables are differently-sized.
    /// @return A generator of the tuples with the zipped values.
    mamba::Generator zip(mamba::Bitset& first, mamba::Bitset& second, bool strict = true);
    /// @brief Imports the module by the name. This is the function invoked by the import statement.
    /// @param name The name of the module to import.
    /// @param globals The dictionary of the global variables to interpret the imported ids.
    /// @param locals The dictionary of the local variables to interpret the imported ids.
    /// @param fromlist If none, the module is imported entirely. If a list, only the
    /// specified names are imported.
    /// @param level The depth of the parent directories to search for the module.
    void __import__(mamba::String& name, mamba::Dictionary* globals, mamba::Dictionary* locals,
                    mamba::List* fromlist, int level = 0);
};