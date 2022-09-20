/* Mozilla Public License v2 | Vladyslav Korol
 * This file is a part of the Python interpreter
 * implementation that contains all the standard
 * aspects that are imbued into the language itself.*/
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <complex.h>
#include "Commons.cc"
namespace Interpreter{

    /// @brief Raises the base to the exponent. A wrapper around C++ pow32x.
    double pow(float base, float exponent){
        return powf32x(base, exponent);
    }

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
        list.insert(object.methods);
        return list;
    }
    /// @brief Enumerates through the given collection.
    /// @return A linked list of the index and value of the collection items.
    std::list<std::pair<int, PyClass>>* enumerate(PyClass collection){
        int count = 0;
        std::list<std::pair<int, PyClass>> *enumeration;
        for (auto item : collection){
            enumeration->push_back(std::pair<int, PyClass>(count, item));
            count++;
        }
        return enumeration;
    }

    void print(std::string text){}

    int len(){}


};