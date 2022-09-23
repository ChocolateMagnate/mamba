
/* Mozilla Public License v2 | Vladyslav Korol
 * This file implements the basic built-in datatypes in Python
 * such as int, float, str, lists dicts, etc. */
#include <iostream>
#include <vector>
#include <tuple>
#include <list>
#include "Dynamism.cc"
namespace Interpreter {

            //###########################

            //    STANDARD DATATYPES    

            //###########################


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
    /// @brief Encapsulates frequently used string searching technique in a separate method.
    /// @param datatable The scope of characters to enumerate through.
    /// @return True if the string contains one of the characters, false otherwise.
    template<int size>
    constexpr bool found(const char* datatable){//Resolve unmodifiable expression error
        for (char character = *(datatable + size); size > 0; size--)
            if (std::find(this->data(), this->data() + this->size(), character))
                return true;
        return false;
    } 
    public:
        //TO-DO: implement array wrapping in the constructors below:
        str(){}
        str(const char*){}
        str(std::string text){}
        /// @brief Attempts to convert Pythonic string into std::string for type safety purposes.
        /// @return std::string equivalent, otherwise throws an exception.
        std::string tryParseIntoString(){

        }
        /// @brief Splits the input string into sustrings by the delimiter.
        /// @param delimeter The string to divide by.
        /// @return A linked list of divided fragments.
        std::list<std::string> split(std::string delimiter){
            std::list<std::string> fragmentedStrings;
            int start = this->find(delimiter);
            while (start != std::string::npos){
                fragmentedStrings.push_back(this->substr(0, start));
                this->replace(start, delimiter.size(), this->substr(start));
                start = this->find(delimiter);
            }
            return fragmentedStrings;
        }
        /// @brief Splits the input string into substrings by the specified separators.
        /// @param delimiters A set of strings to divide by.
        /// @return A linked list of divided fragments.
        std::list<std::string> split(const std::vector<std::string> delimiters){
            std::list<std::string> results;
            for (std::string delimeter : delimiters)
                results.splice(results.end(), split(delimeter));
            return results;
        }
        /// @brief Turns the first character in the string into uppercase.
        void capitalize(){
            std::transform(this->begin(), this->begin(), this->begin(), ::toupper);
        }
        /// @brief Converts string into lowercase.
        void casefold(){
            //TO-DO: implement aggressive differences with lower().
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
            return std::find<int>(this->begin(), this->end(), value);
        }
        /// @brief Verifies if all characters in the string belong to the Latin alphabet.
        /// @return True if all characters are alphabetic, false otherwise.
        bool isalpha(){
            const char alphabet[] = {'Q', 'q', 'W', 'w', 'E', 'e', 'R', 'r', 'T', 't', 
                    'Y', 'y', 'U', 'u', 'I', 'i', 'O', 'o', 'P', 'p', 'A', 'a', 'S', 's', 
                    'D', 'd', 'F', 'f', 'G', 'g', 'H', 'h', 'J', 'j', 'K', 'k', 'L', 'l',
                    'Z', 'z', 'X', 'x', 'C', 'c', 'V', 'v', 'B', 'b', 'N', 'n', 'M', 'm'};
            return found<52>(alphabet);
            //TO-DO: resolve expression unmodifiable error on the line 170.
        }
        /// @brief Verifies if the string consists of only numeric symbols.
        /// @return True if all characters as 0-9, false otherwise.
        bool isnumeric(){
            const char numbers[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};
            return found<10>(numbers);
        }
        /// @brief Verifies is the string characters are alphanumeric.
        /// @return True if all characters are from alphabet or numbers, false otherwise.
        bool isalum(){
            if (!isalpha()) return false;
            return isnumeric();
        }
        /// @brief Verifies if the string only constains ASCII characters.
        /// @return True if all string characters are ASCII symbols, false otherwise.
        bool isascii(){
            if (!isalum()) return false;
            const char characters[] = {' ', '!', '"', '#', '$', '%', '&', '\'',
                '(', ')', '*', '+', ',', '-', '.', '/', ':', ';', '<', '=', '>',
                '?', '@', '[', '\\', ']', '^', '_', '`', '{', '|', '}', '~'};
            return found<33>(characters);
        }
        /// @brief Verifies if the unicode string contains the decimal symbols.
        /// @return True if string only constains digits, false otherwise.
        bool isdecimal(){}
        /// @brief Verifies if the string contains the decimal symbols.
        /// @return True if string only constains digits, false otherwise.
        bool isdigit(){
            const char numbers[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};
            return found<10>(numbers);
        }
        /// @brief Defines if the string is a valid identifier.
        /// @return True if the string can be used as a variable name, false otherwise.
        bool isidentifier(){
            //Verify if the string does not begin with a forbidden character.
            const char banned[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', ' '};
            for (char character : banned)
                if (std::find(this->data(), this->data() + this->size(), character))
                    return false;
            //Verify if the string only constains allowed symbols.
            const char permitted[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '_',
                    'Q', 'q', 'W', 'w', 'E', 'e', 'R', 'r', 'T', 't', 'Y', 'y', 'U', 'u',
                    'I', 'i', 'O', 'o', 'P', 'p', 'A', 'a', 'S', 's', 'D', 'd', 'F', 'f', 
                    'G', 'g', 'H', 'h', 'J', 'j', 'K', 'k', 'L', 'l', 'Z', 'z', 'X', 'x', 
                    'C', 'c', 'V', 'v', 'B', 'b', 'N', 'n', 'M', 'm'};
            return !found<63>(permitted);
        }
        /// @brief Verifies if the whole string is lowercase.
        /// @return True if all characters are lowercase, false otherwise.
        bool islower(){
            const char upper[] = {'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O',
                    'P', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'Z', 'X',
                    'C', 'V', 'B', 'N', 'M'};
            return !found<26>(upper);
        }
        /// @brief Verifies if the string can be printed in its full capacity.
        /// @return True if all characters can be explicitly printed, false otherwise.
        bool isprintable(){
            const char unprintable[] = {'\r', '\n'}; //TO ADD other unprintable symbols.
            return !found<2>(unprintable);
        }
        /// @brief Verifies if the string consists entirely from whitespaces.
        /// @return True if all characters are whitespaces, false otherwise.
        bool iswhitespace(){
            const char spaces[] = {' ', '\t', '\n', '\r'};
            return found<4>(spaces);
        }
        /// @brief Verifies if the string follows the title convention rules.
        /// @return True if all words are uppercase and the rest of the letters
        /// are lowercase, false otherwise.
        bool istitle(){
            auto words = split(std::string(" "));
            for (std::string word : words){
                //Verify the first letter is uppercase.
                switch (word[0]){
                    case 'Q': case 'W': case 'E': case 'R': case 'T': case 'Y': case 'U': 
                    case 'I': case 'O': case 'P': case 'A': case 'S': case 'D': case 'F':
                    case 'G': case 'H': case 'J': case 'K': case 'L': case 'Z': case 'X':
                    case 'C': case 'V': case 'B': case 'N': case 'M':
                        break;
                    default:
                        return false;
                }
                //Verify the rest of the letters are lowercase.
                for (int size = word.size(); size > 1; size--){
                    switch (word[size]){
                        case 'Q': case 'W': case 'E': case 'R': case 'T': case 'Y': case 'U': 
                        case 'I': case 'O': case 'P': case 'A': case 'S': case 'D': case 'F':
                        case 'G': case 'H': case 'J': case 'K': case 'L': case 'Z': case 'X':
                        case 'C': case 'V': case 'B': case 'N': case 'M':
                            return false;
                        default:
                            continue;
                    }
                }
            }
        }
        /// @brief Serialises the input collection into string representations
        /// and joins them with one string separated by its content.
        /// @param enumerable The collection to join.
        /// @return A joined string.
        str join(PyCollection enumerable){
            std::string base, result;
            for (auto character = this->begin(); character < this->end(); character++)
                base += *character; //Saving the initial state of the string to append later.
            std::list<std::string> collection;
            for (PyGenericObject item : enumerable) collection.pop_back(item.__str__());
            for (std::string item : collection) result += item + base;
            return result;
        }
        /// @brief Produces a new aligned string where the unfilled space is replaced.
        /// @param distance The length of the new string.
        /// @param symbol The symbol to replace the empty space with.
        /// @return A new aligned string.
        str ljust(int distance, char symbol = ' '){
            if (this->size() >= distance) return *this;
            int gap = distance - this->size();
            return *this + std::string(gap, symbol);
        }
        /// @brief Converts the string into lowercase.
        void lower(){
            std::transform(this->begin(), this->end(), this, ::tolower);
        }
        /// @brief Removes any occurrance of the given characters from the beginning.
        /// @param characters The characters to delete, by convention is a space.
        /// @return A new trimmed Pythonic string.
        str lstrip(std::string characters = " "){
            int end = 0;
            char symbol = characters[end];
            while (characters.find(symbol) != std::string::npos) end++;
            return str(this->substr(end));
        }
        /// @brief Replaces the characters in the string.
        /// @param base The string to replace.
        /// @param replace The string to replace base with.
        /// @param deletion Optionally, what characters erase from the string entirely.
        /// @return A new processed Pythonic string.
        str maketrans(std::string base, str replacement = "", str deletion = ""){
            str newstring = *this;
            int lengthOfString = base.size();
            int indexOfString = this->find(base);
            while (indexOfString != std::string::npos){
                newstring.replace(indexOfString, lengthOfString, replacement);
                indexOfString = newstring.find(base);
            }
            int indexOfDeletion = this->find(deletion);
            if (indexOfDeletion != std::string::npos || deletion != "")
                return maketrans(newstring, deletion);
            return newstring;
        }
        /// @brief Replaces the characters in the string.
        /// @param dictionary The dictionary whose keys to replace with values.
        /// @return A new processed Python string.
        str maketrans(dict dictionary){
            str result = *this;
            for (std::pair<std::string, std::string> pair : dictionary)
                result = result.maketrans(pair.first, pair.second);
            return result;
        }
        /// @brief Replaces the gaps in the object with the specified values in O(2N).
        /// @param gaps The vector of fragments to replace gaps with.
        /// @return Formatted Pythonic string with the filled gaps.
        void format(std::vector<std::string> gaps){
            int index, rows = 0;
            std::string enumeration = *this;
            std::list<std::array<int, 3>> table; //Start, end, gap.
            //Validate the string and generate the index table.
            for (std::string gap : gaps){
                bool wrongSymbol = false;
                int start = enumeration.find("{");
                int  end  = enumeration.find("}");
                if (start + 1 != end) {
                    //If braces are empty like {}, then we proceed to the next 
                    //part, otherwise we extract the index of gap inside them.
                    std::string medium = enumeration.substr(start, end);
                    for (char symbol : medium){
                        switch (symbol){ //Make sure there are only numerals in the braces.
                            case '0': case '1': case '2': case '3': case '4': 
                            case '5': case '6': case '7': case '8': case '9': 
                            case ' ': case '\0': //Empty braces imply the proper order.
                                break;
                            default:
                                wrongSymbol = true;
                                break;
                        }
                        if (wrongSymbol) throw "Gaps can only contain indexes.";
                    }
                    table.push_back({start, end, atoi(medium.c_str())});
                } else table.push_back({start, end, rows});
                enumeration = enumeration.substr(end + 1);
                rows++;
            }
            //Replace the gaps with the indexed rows.
            for (std::array<int, 3> row : table){
                std::string::iterator start = this->begin() + std::get<0>(row);
                std::string::iterator  end  = this->begin() + std::get<1>(row);
                this->replace(start, end, gaps[std::get<2>(row)]);
            }
        }

};
/// @brief The lazy iterator range class in Python.
class range{
    //The current property is isolated to not be messed with by the developer's mistake.
    double current;
    public:
        double start, end, step;
        range(int finish){
            end = finish;
            start = 0; step = 1;
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
};