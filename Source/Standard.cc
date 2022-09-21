/* Mozilla Public License v2 | Vladyslav Korol
 * This file is a part of the Python interpreter
 * implementation that contains all the standard
 * aspects that are imbued into the language itself.*/
#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <complex.h>
#include "Commons.cc"
namespace Interpreter{
    /// @brief Describes the blueprint for custom classes in Python.
    class PyClass{
        /* This class represents user-defined types in Python as dictionaries
         * from member names to their description, which includes types and 
         * argument lists. Implements dynamic dispatch and prototyping.*/
        public:
            std::list<//List of different constructors
                std::pair<//Pairs of argument lists and the function
                    std::list<std::string>, std::list<unsigned int>>> constructors;
            std::map<//Access modifiers
                std::string, std::map<//Method names and
                    std::string, std::vector<//argument list.
                        std::string>>> methods;
            std::map<//Access modifiers
                std::string, std::map<//Names and types.
                    std::string, std::string>> properties;
            PyClass(
                std::map<
                    std::string, std::map<
                        std::string, std::vector<
                            std::string>>> methods,
                std::map<
                    std::string, std::map<
                        std::string, std::string>> properties
            ){
                this->methods = methods;
                this->properties = properties;
            }
            /// @brief Initialises a new object by prototyping the class template.
            /// @return A new object of the describes class.
            PyClass prototype(std::vector<std::pair<PyGenericObject, std::string>> arguments){

            }

    };
    /// @brief The primary wrapper around any object in Python that describes
    /// it with a dictionary of properties and methods sorted by access modifiers.
    class PyGenericObject{
        /* This class represents objects as dictionaries of their members
         * sorted by the access modifiers. Methods are encoded with vectors
         * of unsigned integers that correlate the the main execution flow,
         * and properties are encoded into numeric representation that is to 
         * parse into the actual data.*/
        public:
            std::map<
                std::string, std::map<
                    std::string, unsigned int>> properties;
            std::map<
                std::string, std::map<
                    std::string, std::pair<
                        std::vector<std::string>, std::vector<unsigned int>>>> methods;
            PyGenericObject(
                std::map<
                    std::string, std::map<
                        std::string, unsigned int>> properties,
                std::map<
                std::string, std::map<
                    std::string, std::pair<
                        std::vector<std::string>, std::vector<unsigned int>>>> methods){
                            this->properties = properties;
                            this->methods = methods;
            }
            /// @brief Python constructor that deals with assigning all values to the properties.
            void __init__(){}
            /// @brief Gets the string representation of the object.
            /// @return The string to be printed in the terminal.
            std::string __str__(){
                const void* pointer = static_cast<const void*>(this);
                std::stringstream ss;
                ss << pointer;
                return ss.str();
            }

            std::string __repr__(){}


    };

    class PyCollection : public std::iterator<PyGenericObject, int>{
        protected:
            int count;
        public:
            int size(){
                return count;
            }
            virtual std::iterator<PyGenericObject, int> begin();
            virtual std::iterator<PyGenericObject, int> end();
            virtual std::iterator<PyGenericObject, int> advance();


    };
    /// @brief The base data structure class that encompasses the essential
    /// operations shared between all basic collections that is inherited. 
    class tuple : public PyCollection {
        protected:
            int count = -1;
            std::vector<PyGenericObject> items;
        public:
            PyGenericObject* operator[](int index){
                return &items[index];
            }
            PyGenericObject* next(){
                count++;
                return &items[count];
            }
            virtual void append(PyGenericObject value){
                items.push_back(value);
                count++;
            }
            bool contains(PyGenericObject value){
                if (std::find(items.begin(), items.end(), value) != items.end()) return true;
                return false;
            }
    };
    /// @brief The utility class used to represent the subset of data
    /// structures as a vector of generic objects available by indexes.
    class list: public tuple {
        public:
            void remove(PyGenericObject value){}
            PyGenericObject pop(PyGenericObject value){}
            void clear(){
                items.clear();
            }
    };
    /// @brief Unordered items containing only unique values.
    class set : public list {
        public:
            void append(PyGenericObject value) override {
                if (contains(value)) throw "Sets must contain only unique values.";
            }
    };
    /// @brief Python dictionary class represented as a wrapper around C++ std::map.
    class dict : public PyCollection {
        public:
            //Keys must be stored individually because std::map does not 
            //provide any public interface to access them, but Python does.
            std::vector<PyGenericObject> keys;
            std::map<PyGenericObject, PyGenericObject> content;
            //Settting a new key-value pair:
            PyGenericObject operator[](std::pair<PyGenericObject, PyGenericObject> value){
                content[value.first] = value.second;
                keys.push_back(value.first);
                count++;
            }  
            //Getting value from the key:
            PyGenericObject operator[](PyGenericObject key){
                return content[key];
            }
    };
    /// @brief Pythonic wrapper around std::string with additional utilities.
    class str : public std::string {
        public:
            /// @brief Splits the input string into sustrings by the delimiter.
            /// @param delimeter The string to divide by.
            /// @return A linked list of divided fragments.
            std::list<std::string> split(PyGenericObject delimiter){
                std::string text = delimiter.__str__();
                std::list<std::string> fragmentedStrings;
                int start = this->find(text);
                while (start != std::string::npos){
                    fragmentedStrings.push_back(text.substr(0, start));
                    text = text.substr(start);
                    start = this->find(text);
                }
                return fragmentedStrings;
            }
            /// @brief Splits the input string into substrings by the specified separators.
            /// @param delimiters A set of strings to divide by.
            /// @return A linked list of divided fragments.
            std::list<std::string> split(std::vector<PyGenericObject> delimiters){
                std::list<std::string> results;
                for (PyGenericObject delimeter : delimiters)
                    results.splice(results.end(), split(delimeter));
                return results;
            }
            /// @brief Turns the first character in the string into uppercase.
            void capitalize(){
                std::transform(this->begin(), this->begin(), this->begin(), ::toupper);
            }
            /// @brief Converts string into lowercase.
            void casefold(){
                std::transform(this->begin(), this->end(), this, ::tolower);
            }
            /// @brief Counts the amount of times specific value occurs in the string.
            /// @param repeatable The value that encounters in the string.
            /// @return The number of occurrences.
            int count(std::string repeatable){
                int amount = 0;
                int search = this->find(repeatable);
                while (search != std::string::npos){
                    repeatable = repeatable.substr(0, search);
                    search = this->find(repeatable);
                    amount++;
                }
                return amount;
            }
            /// @brief Verifies if the string ends with the given character. 
            /// @return True if the last symbol is the character, otherwise false.
            bool endswith(char character){
                if (this->back() == character) return true;
                return false;
            }
            /// @brief The wrapper around std::find.
            /// @return The position of the first character of the occurrence found, otherwise -1.
            int find(std::string value){
                return this->find(value);
            }
            /// @brief Verifies if all characters in the string belong to the Latin alphabet.
            /// @return True if all characters are alphabetic, false otherwise.
            bool isalpha(){
                const char alphabet[] = {'Q', 'q', 'W', 'w', 'E', 'e', 'R', 'r', 'T', 't', 
                        'Y', 'y', 'U', 'u', 'I', 'i', 'O', 'o', 'P', 'A', 'a', 'S', 's', 
                        'D', 'd', 'F', 'f', 'G', 'g', 'H', 'h', 'J', 'j', 'K', 'k', 'L', 'l',
                        'Z', 'z', 'X', 'x', 'C', 'c', 'V', 'v', 'B', 'b', 'N', 'n', 'M', 'm'};
                for (char character : this->begin())
                    if (!std::find(alphabet[0], alphabet[50], character)) return false;
                return true;
            }
            /// @brief Verifies is the string characters are alphanumeric.
            /// @return True if all characters are from alphabet or numbers, false otherwise.
            bool isalum(){
                if (!isalpha()) return false;
                const char numbers[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};
                for (char character : this->begin())
                    if (!std::find(numbers[0], numbers[9], character)) return false;
                return true;
            }
            /// @brief Verifies if the string only constains ASCII characters.
            /// @return True if all string characters are ASCII symbols, false otherwise.
            bool isascii(){
                if (!isalnum()) return false;
                const char characters[] = {' ', '!', '"', '#', '$', '%', '&', '\'',
                    '(', ')', '*', '+', ',', '-', '.', '/', ':', ';', '<', '=', '>',
                    '?', '@', '[', '\\', ']', '^', '_', '`', '{', '|', '}', '~'};
                for (char character : this->begin())
                    if (!std::find(characters[0], characters[32], character)) return false;
                return true;
            }
            /// @brief Verifies if the unicode string contains the decimal symbols.
            /// @return True if string only constains digits, false otherwise.
            bool isdecimal(){}
            /// @brief Verifies if the string contains the decimal symbols.
            /// @return True if string only constains digits, false otherwise.
            bool isdigit(){
                const char numbers[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};
                for (char character : this->begin())
                    if (!std::find(numbers[0], numbers[9], character)) return false;
                return true;
            }
            /// @brief Defines if the string is a valid identifier.
            /// @return True if the string can be used as a variable name, false otherwise.
            bool isidentifier(){
                //Verify if the string does not begin with a forbidden character.
                const char banned[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', ' '};
                for (char character : banned)
                    if (std::find(this->data(), this->data() + this->size(), character)) return false;
                //Verify if the string only constains allowed symbols.
                const char permitted[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '_',
                        'Q', 'q', 'W', 'w', 'E', 'e', 'R', 'r', 'T', 't', 'Y', 'y', 'U', 'u',
                        'I', 'i', 'O', 'o', 'P', 'A', 'a', 'S', 's', 'D', 'd', 'F', 'f', 'G',
                        'g', 'H', 'h', 'J', 'j', 'K', 'k', 'L', 'l', 'Z', 'z', 'X', 'x', 'C',
                        'c', 'V', 'v', 'B', 'b', 'N', 'n', 'M', 'm'};
                for (char character : permitted) 
                    if (!std::find(this->data(), this->data() + this->size(), character)) return false;
                return true;
            }

            
    };

    /// @brief Verifies if the boolean collection only contains true values.
    /// @return True if all items are true, false otherwise.
    bool all(std::vector<bool> sequence){
        for (bool item : sequence) if (!item) return false;
        return true;
    }
    /// @brief Verifies if there is a true value in the sequence.
    /// @return True if the collection contains at least one true.
    bool any(std::vector<bool> sequence){
        for (bool item : sequence) if (item) return true;
        return false;
    }
    /// @brief Lists all properties and methods of the given object.
    /// @param object The Pythonic object whose public members to list.
    /// @return The list of public properties and methods.
    auto dir(PyClass object){
        auto list = object.properties;
        list.insert(list.end(), object.methods.begin(), object.methods.end());
        return list;
    }
    /// @brief Enumerates through the given collection.
    /// @return A linked list of the index and value of the collection items.
    std::list<std::pair<int, PyGenericObject>>* enumerate(PyCollection collection){
        int count = 0;
        std::list<std::pair<int, PyGenericObject>> *enumeration;
        for (auto item : collection){
            enumeration->push_back(std::pair<int, PyGenericObject>(count, item));
            count++;
        }
        return enumeration;
    }
    /// @brief Takes user inout as a string from the terminal optionally printing a message before.
    /// @return The string output typed by user.
    std::string input(std::string message){
        std::string slot;
        std::cout << message;
        std::cin >> slot;
        std::cout << "\n";
        return slot;
    }
    /// @brief Verifies if the specified object is an instance of the input class.
    /// @return True if object belongs to the class, otherwise false.
    bool isinstance(PyClass object, std::string _class){}

    /// @brief Evaluates the length of the given collection object.
    /// @return The amount of items data structure currently holds.
    int len(PyCollection object){
        return object.size();
    }
    /// @brief Seeks for the largest value in the collection.
    /// @return The biggest numeric value.
    double max(PyCollection items){
        double biggest = 0;
        for (auto item : items){
            if (item.isNumeric){
                if (item.numeric > biggest) biggest = item.numeric;
            }
        }
        return biggest;
    }
    /// @brief Seeks the least numeric value in the collection.
    /// @return The smallest number given in the input.
    double min(PyCollection items){
        double least = 0;
        for (auto item : items){
            if (item.isNumeric){
                if (item.numeric < least) least = item.numeric; 
            }
        }
        return least;
    }
    /// @brief Extracts and returns the next value in the collection if applicable.
    PyClass next(PyCollection objects){}

    /// @brief Raises the base to the exponent. A wrapper around C++ pow32x.
    double pow(float base, float exponent){
        return powf32x(base, exponent);
    }
    /// @brief The lazy iterator range class in Python.
    class range{
        //The current property is isolated to not be messed with by the developer's mistake.
        double current;
        public:
            double start, end, step;
            range(int finish){
                end = finish;
                start = 0;
                step = 1;
            }
            range(double begin, double finish, double go){
                start = begin;
                end = finish;
                step = go;
            }
            std::pair<double, bool> next(){
                auto result = std::pair<double, bool>(current, true);
                current += step;
                if (current >= end){
                    result.second = false;
                }
                return result;
            }
    };
    /// @brief Prints the object into the standard output.
    void print(PyGenericObject object){
        std::string text = object.__str__();
        if (text != "") std::cout << text << "\n";
        else std::cout << object.__repr__();
    }


};