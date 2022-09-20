/* Mozilla Public License v2 | Vladyslav Korol
 * This file is a part of the Python interpreter
 * implementation that contains all the standard
 * aspects that are imbued into the language itself.*/
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <list>
#include <complex.h>
#include "Commons.cc"
namespace Interpreter{
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
            
    };

    class PyCollection{
        public:
            int count;
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
    std::list<std::pair<int, PyClass>>* enumerate(PyCollection collection){
        int count = 0;
        std::list<std::pair<int, PyClass>> *enumeration;
        for (auto item : collection){
            enumeration->push_back(std::pair<int, PyClass>(count, item));
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
        return object.count;
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

    void print(std::string text){}


};