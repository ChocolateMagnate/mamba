/* Mozilla Public License v2 | Vladyslav Korol
* Python Standard includes a verity of predefined classes and functions
* that are automatically included by the interpreter itself. This file 
* implements dynamic typisation through PyGenericObject, custom user classes,
* inheritance, polymorphism, abstraction as blueprint dictionaries.*/
#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <complex.h>
#include "Source/Commons.cc"
namespace Interpreter{

            //###########################

            //    STANDARD DYNAMISM     

            //###########################

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
        std::string type;
        PyClass* typeReference;
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
        // OBJECT INITIALISATION    
        /// @brief Generates a new instance of the object.
        /// @param cls The template of the object. Using class by default.
        /// @return A new uninitialised object.
        PyClass __new__(PyClass cls){}
        /// @brief Python constructor that deals with assigning all values to the properties.
        void __init__(){}
        /// @brief Removes the object from the memory in the process of garbage collection.
        void __del__(){}

        //UNARY OPERATIONS 

        //TYPE CONVERSION  
        /// @brief Converts the object to an integer representations.
        PyInt __int__(){}
        /// @brief Converts the object into a real number representation.
        PyFloat __float__(){}
        PyComplex __complex__();



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

class PyInt : public PyGenericObject{
    public:
        int base;
        std::string __str__(){
            return std::to_string(base);
        }
        int operator+(int integer){
            return base + integer;
        }
        double operator+(double real){
            return base + real;
        }
};

class PyFloat : public PyGenericObject{
    public:
        double base;
        std::string __str__(){
            return std::to_string(base);
        }
        double operator+(double real){
            return base + real;
        }
};

class PyComplex : public PyGenericObject {};
/// @brief Generic interface for Python data structures used to enumerate through items in it.
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
/// @brief Verifies if the input collection contains the search items.
/// @param collection The object to search in.
/// @param search The data to search.
/// @return True if collection contains at least one search item, otherwise false.
bool contains(PyCollection* collection, PyCollection* search){
    for (PyGenericObject item : collection)
        for (PyGenericObject searchItem : search)
            if (std::find(search->begin(), search->end(), searchItem) != collection->end())
                return true;
    return false;
}

PyComplex PyGenericObject::__complex__(){

}
};