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
        PyClass(){
            throw "Cannot create an empty class.";
        }
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
                    // OBJECT INITIALISATION    
        /// @brief Generates a new instance of the object.
        /// @param cls The template of the object. Using class by default.
        /// @return A new uninitialised object.
        PyClass __new__(PyClass cls){}
        /// @brief Python constructor that deals with assigning all values to the properties.
        void __init__(){}
        /// @brief Removes the object from the memory in the process of garbage collection.
        void __del__(){}

                    // ATTRIBUTE MANAGEMENT
        /// @brief Is used to add an initialise a new class attribute.
        void __setattr__(){}
        /// @brief Deletes certain property from class.
        void __delattr__(){}
};
/// @brief The primary wrapper around any object in Python that describes
/// it with a dictionary of properties and methods sorted by access modifiers.
class PyGenericObject : public PyClass {
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
        PyGenericObject(){
            this->type = "object";
            this->typeReference = nullptr;
        }
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
                        //TO-DO: Add type inference.
        }

                    //UNARY OPERATIONS 

                    //TYPE CONVERSION  
        /// @brief Converts the object to an integer representations.
        PyInt __int__(){
            try {return PyInt(this->properties["public"]["value"]);}
            catch (const char* e) {throw "Cannot convert to integer.";}
        }
        /// @brief Converts the object into a real number representation.
        PyFloat __float__(){
            try {return PyFloat(this->properties["public"]["value"]);}
            catch (const char* e) {throw "Cannot convert to float.";}
        }
        PyComplex __complex__(){
            try {return PyComplex(this->properties["public"]["value"]);}
            catch (const char* e) {throw "Cannot convert to complex.";}
        }

                 // STRING MANIPULATION 
        /// @brief Gets the string representation of the object.
        /// @return The string to be printed in the terminal.
        std::string __str__(){
            const void* pointer = static_cast<const void*>(this);
            std::stringstream ss;
            ss << pointer;
            return ss.str();
        }
        /// @brief Converts the object into machine-friendly string representation.
        /// @return The string used primarily by interpreter.
        std::string __repr__(){}
        /// @brief Converts the object into a unicode representation.
        /// @return Unicode string used by the unicode() function.
        std::string __unicode__(){}
        /// @brief Formats the objects into string representation used by str.format() method.
        std::string __format__(std::string format){}
        /// @brief Converts object into a hash.
        /// @return The encoded form of the object that cannot be decoded but can be verified.
        std::string __hash__(){}
        /// @brief Formats the object as a boolean value.
        /// @return True or false.
        bool __nonzero__(){}
        /// @brief Gets the size of the object that can be used for arbitrary purposes.
        /// @return The size in any units designed by class writers.
        int __sizeof__(){}

                // OPERATOR OVERLOADING 
        /// @brief Overloads the + operator.
        PyGenericObject __add__(){}
        /// @brief Overloads - operator.
        PyGenericObject __sub__(){}
        /// @brief Overloads the * operator.
        PyGenericObject __mul__(){}
        /// @brief Overloads the // operator.
        PyGenericObject __flordiv__(){}
        /// @brief Overloads the / operator.
        PyGenericObject __truediv__(){}
        /// @brief Overloads the % operator.
        PyGenericObject __mod__(){}
        /// @brief Overloads the ** operator.
        PyGenericObject __pow__(){}
        /// @brief Overloads the < operator.
        PyGenericObject __lt__(){}
        /// @brief Overloads the <== operator.
        PyGenericObject __le__(){}
        /// @brief Overloads the == operator.
        PyGenericObject __eq__(){}
        /// @brief Overloads the != operator.
        PyGenericObject __ne__(){}
        /// @brief Overloads the >= operator.
        PyGenericObject __ge__(){}
};
#define PyCollection std::vector<PyGenericObject>
/// @brief General interface for overloading 
/// operators regarding arithmetic operations.
/// @tparam type The numeric datatype: int, float, complex.
template<typename type> class INumeric{
    type base;
    std::string __str__(){
        return std::to_string(base);
    }
    type operator+(double addition){
        return base + addition;
    }
    type operator-(double subtraction){
        return base - subtraction;
    }
    type operator*(double multiplication){
        return base * multiplication;
    }
    type operator/(double division){
        return base / division;
    }
    type wholeDivide(double division){ // // operator
        return round(base / division);
    }
    type operator%(double modulo){
        return fmod(base, modulo);
    }
    type operator^(double power){
        return pow(base, power);
    }
    bool operator<(double comparison){
        return base < comparison;
    }
    bool operator<=(double comparison){
        return base <= comparison;
    }
    bool operator==(double comparison){
        return base == comparison;
    }
    bool operator!=(double comparison){
        return base != comparison;
    }
    bool operator>=(double comparison){
        return base >= comparison;
    }
    bool operator>(double comparison){
        return base > comparison;
    }
    type operator+=(double addition){
        return base += addition;
    }
    type operator-=(double subtraction){
        return base -= subtraction;
    }
    type operator*=(double multiplication){
        return base *= multiplication;
    }
    type operator/=(double division){
        return base /= division;
    }
    type operator%=(double modulo){
        return fmod(base, modulo);
    }
    type operator^=(double power){
        return pow(base, power);
    }
    type operator++(int){
        return base++;
    }
    type operator--(int){
        return base--;
    }

};
class PyBool : public PyGenericObject{
    public:
        bool base;
        PyBool(std::string condition){
            if (condition == "True") this->base = true;
            else this->base = false;
            this->type = "bool";
            this->typeReference = nullptr;
            this->properties["public"]["value"] = this->base ? 1 : 0;
        }
};
//These are the basic numeric datatypes used in Python.
class PyInt : public PyGenericObject, public INumeric<int>{};
class PyFloat : public PyGenericObject, public INumeric<double>{};
class PyComplex : public PyGenericObject, public INumeric<std::complex<float>>{};

/// @brief Verifies if the input collection contains the search items.
/// @param collection The object to search in.
/// @param search The data to search.
/// @return True if collection contains at least one search item, otherwise false.
bool contains(PyCollection* collection, PyCollection* search){
    for (PyGenericObject item : *collection)
        for (PyGenericObject searchItem : *search)
            if (std::find(search->begin(), search->end(), searchItem) != collection->end())
                return true;
    return false;
}


};