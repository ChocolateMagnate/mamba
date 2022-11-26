/* Mamba Python by Vladyslav Korol | Apache License 2.0
 * This file deals with implementing the built-in functions
 * imbued into the interpreter itself, such as print(), dir(), etc.*/
#include <iostream>
#include "./Interpreter/Base.hh"
#include "Types.cc"
namespace mamba {
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
auto dir(mamba::PyObject object){
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
std::string input(std::string message = ""){
    std::string slot;
    std::cout << message;
    std::cin >> slot;
    std::cout << "\n";
    return slot;
}
/// @brief Verifies if the specified object is an instance of the input class.
/// @return True if object belongs to the class, otherwise false.
bool isinstance(PyClass object, std::string _class){}
/// @brief Seeks for the largest value in the collection.
/// @return The biggest numeric value.
double max(PyCollection items){
    double biggest = 0;
    for (auto item : items)
        if (item.__int__() > biggest) biggest = item.__int__();
    return biggest;
}
/// @brief Seeks the least numeric value in the collection.
/// @return The smallest number given in the input.
double min(PyCollection items){
    //TODO: take care of the type cohesion.
    PyFloat least = PyFloat(0);
    for (auto item : items)
        if (item.__int__() < least) least = item.__int__(); 
    return least;
}
/// @brief Extracts and returns the next value in the collection if applicable.
PyGenericObject next(PyCollection objects){}

/// @brief Raises the base to the exponent. A wrapper around C++ pow32x.
double pow(float base, float exponent){
    return powf32x(base, exponent);
}
/// @brief Evaluates the length of the given collection object.
/// @return The amount of items data structure currently holds.
int len(PyCollection object){
    return object.size();
}
/// @brief Prints the object into the standard output.
void print(PyGenericObject object){
    std::string text = object.__str__();
    if (text != "") std::cout << text << "\n";
    else std::cout << object.__repr__();
}
/// @brief Gets the type of the object as a string representation.
str type(PyGenericObject object){
    return std::string("<class ") + object.type + std::string(">");
}
};
