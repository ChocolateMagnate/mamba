/* Mamba Python by Vladyslav Korol | Apache License 2
 * This file describes the core components of the 
 * runtime enviornment that enable dynamic typing and
 * the foundational datatypes with bitsets. */

#pragma once
#include <string>
namespace mamba {
    /// @brief Basic representation of Python function as a functor object.
    class Function {
        public:
            /// @brief The function to be executed.
            /// @param args The arguments to be passed to the function.
            /// @return The result of the function.
            virtual void operator()(const char** args);
    };


    /// @brief Describes Pythonic lazy generators and iterators.
    class Generator {
        public:
            Generator(Function& function);
            Bitset generate(); //Yields the next value.
    };

    /// @brief The basic bitset used to represent fluid data.
    class Bitset {
        public:
            Bitset(); //Default constructor.
            template<int size> Bitset(); //Constructor for a bitset of a given size.
            Bitset(const Bitset& other); //Copy constructor.
            Bitset(Bitset&& other); //Move constructor.
            Bitset& operator=(const Bitset& other); //Copy assignment operator.
            Bitset& operator=(Bitset&& other); //Move assignment operator.
            ~Bitset(); //Destructor.

            unsigned int size() const; //Returns the size of the bitset.
            //To be continued...

    };

    /// @brief Implements absolute-precision floating point numbers as a bitset.
    class Float : public Bitset {};

    class String : public Bitset {
        public:
            String(unsigned int size = 2);
            String(const char* value);
            String(const std::string& value);
            String(const String& other);
            String(const String&& other);
            String(const Bitset& other); //String conversion constructor.
            String& operator=(const String& other);
            String& operator=(String&& other);
            ~String();

        /// @brief Concatenates the string with another string.
        /// @param other The other string to concatenate with.
        /// @return The concatenated string.
        String operator+(const String& other) { }
        /// @brief Extracts all external characters from the string.
        /// @param other The string containing the characters to be removed.
        /// @return The serialised string.
        String operator-(const String& other);
        /// @brief Extracts all external characters from the string.
        /// @param other Constant array of characters to be removed.
        /// @return The serialised string.
        String operator-(const char* other);
        /// @brief Repeats the string a given number of times.
        /// @param count The number of times to repeat.
        /// @return The looped string.
        String operator*(const unsigned int count);
        /// @brief Extends the string with another string.
        /// @param other The string to concatenate with the original.
        void operator+=(const String& other);
        /// @brief Extends the string with another string.
        /// @param other The constant array of characters to concatenate with the original.
        void operator+(const char* other);
        /// @brief Removes characters from the original string.
        /// @param other The string containing the characters to be removed.
        void operator-=(const String& other);
        /// @brief Removes characters from the original string.
        /// @param other The constant array of characters to be removed.
        void operator-=(const char* other);
        /// @brief Loops the string a given number of times.
        /// @param count The number of times to repeat.
        void operator*=(const unsigned int count);
        /// @brief Evaluates two Mamba strings for equality.
        /// @param other The constant array of characters to compare with.
        /// @return True if the strings are equal, false otherwise.
        bool operator==(const String& other);
        /// @brief Evaluates two Mamba strings for equality.
        /// @param other The constant array of characters to compare with.
        /// @return True if the strings are equal, false otherwise.
        bool operator==(const char* other);
        /// @brief Evaluates two Mamba strings for inequality.
        /// @param other The other string to compare with.
        /// @return False if the strings are equal, true otherwise.
        bool operator!=(const String& other);
        /// @brief Evaluates two Mamba strings for inequality.
        /// @param other The constant array of characters to compare with.
        /// @return False if the strings are equal, true otherwise.
        bool operator!=(const char* other);
        /// @brief Evaluates if the string is less than another string.
        /// @param other The string to compare with.
        /// @return True if the original is less than the other, false otherwise.
        bool operator<(const String& other);
        /// @brief Evaluates if the string is less than another string.
        /// @param other The constant array of characters to compare with.
        /// @return True if the original is less than the other, false otherwise.
        bool operator<(const char* other);
        /// @brief Evaluates if the string is greater than another string.
        /// @param other The string to compare with.
        /// @return True if the original is greater than the other, false otherwise.
        bool operator>(const String& other);
        /// @brief Evaluates if the string is greater than another string.
        /// @param other The constant array of characters to compare with.
        /// @return True if the original is greater than the other, false otherwise.
        bool operator>(const char* other);
        /// @brief Evaluates if the string is less than or equal to another string.
        /// @param other The string to compare with.
        /// @return True if the original is less than or equal to the other, false otherwise.
        bool operator<=(const String& other);
        /// @brief Evaluates if the string is less than or equal to another string.
        /// @param other The constant array of characters to compare with.
        /// @return True if the original is less than or equal to the other, false otherwise.
        bool operator<=(const char* other);
        /// @brief Evaluates if the string is greater than or equal to another string.
        /// @param other The string to compare with.
        /// @return True if the original is greater than or equal to the other, false otherwise.
        bool operator>=(const String& other);
        /// @brief Evaluates if the string is greater than or equal to another string.
        /// @param other The constant array of characters to compare with.
        /// @return True if the original is greater than or equal to the other, false otherwise.
        bool operator>=(const char* other);
        /// @brief Accesses a character in the string.
        /// @param count The index of the character to access.
        /// @return The reference to the character that can be read or written.
        char& operator[](const int count);
        /// @brief Slices the string with range-based indexing.
        /// @param range The range of characters to slice.
        /// @return The reference to the sliced string that can be read or written.
        String& operator[](const Generator& range); //String slicing

        /// @brief Returns the length of the string in characters.
        unsigned int size() const;
        /// @brief Returns the constant array of characters equivalent to the string.
        const char* toConstChar() const;
        /// @brief Divides the string into a list of substrings.
        /// @param delimiters The Mamba String separators to use for the division.
        /// @return A list of strings divided by the delimiters.
        Bitset split(const String& delimiters);
        /// @brief Divides the string into a list of substrings.
        /// @param delimiters The constant two-dimensional array
        /// of characters to use for the division.
        /// @return A list of strings divided by the delimiters.
        Bitset split(const char** delimiters);
        /// @brief Turns all characters in the string to uppercase.
        void capitalize();
        /// @brief 
        void casefold();
        /// @brief Identifies if the string ends with an instance of the suffix.
        bool endswith(const String& suffix);
        bool endswith(const char* suffix);
        unsigned int find(const String& sub, const unsigned int start, const unsigned int end);
        unsigned int find(const char* sub, const unsigned int start, const unsigned int end);
        bool isalpha();
        bool isalnum();
        bool isascii();
        bool isnumeric();
        bool isdecimal();
        bool isdigit();
        bool isidentifier();
        bool islower();
        bool isprintable();
        bool isspace();
        bool istitle();
        bool isupper();
        void join(const String& iterable);
        void join(const char** iterable);
        unsigned int len();
        String ljust(const unsigned int width, const char fillchar);
        String ljust(const unsigned int width, const String& fillchar);
        void lower();
        String lstrip(const String& chars);
        String lstrip(const char* chars);
        String maketrans(const String& base, const String& makechars, const String& deletechars = "");
        String maketrans(const char* base, const char* makechars, const char* deletechars = "");
            //To add the dictionary maketrans overload.
    };

    class Dictionary : public Bitset {
        public:
            Dictionary();
            Dictionary(const Dictionary& other);
            Dictionary(const Dictionary&& other);
            Dictionary(const Bitset& other); //Dictionary conversion constructor.
            Dictionary& operator=(const Dictionary& other);
            Dictionary& operator=(Dictionary&& other);
            ~Dictionary();

            Dictionary operator+(const Dictionary& other);
            Dictionary operator-(const Dictionary& other);
            Dictionary operator*(const Dictionary& other);
            Dictionary operator/(const Dictionary& other);
            bool operator|(const Dictionary& other);
            Dictionary operator<<(const Dictionary& other);
            Dictionary operator>>(const Dictionary& other);
            Dictionary operator+=(const Dictionary& other);
            Dictionary operator-=(const Dictionary& other);
            Dictionary operator*=(const Dictionary& other);
            Dictionary operator/=(const Dictionary& other);
            void operator<<=(const Dictionary& other);
            void operator>>=(const Dictionary& other);
            bool operator==(const Dictionary& other);
            bool operator!=(const Dictionary& other);
            bool operator<(const Dictionary& other);
            bool operator>(const Dictionary& other);
            void operator<=(const Dictionary& other);
            void operator>=(const Dictionary& other);
            Bitset operator[](const String& key);
            Bitset operator[](const char** key);
            void operator~();

            unsigned int size() const;
            Bitset keys();
            Bitset values();
            Bitset items();
            void clear();
            void copy();
            void fromkeys(const Bitset& keys, const Bitset& value);
            void get(const Bitset& key);
            void pop(const Bitset& key);
            void popitem();
            void setdefault(const Bitset& key);
            void update(const Bitset& other);
            void update(const Bitset& other, const Bitset& other2);
    };

    /// @brief Describes the built-in list class.
    class List : public Bitset {
        public:
            List();
            List(const List& other);
            List(const List&& other);
            List(const Bitset& other); //List conversion constructor.
            List& operator=(const List& other);
            List& operator=(List&& other);
            ~List();

            List operator+(const List& other);
            List operator-(const List& other);
            List operator*(const List& other);
            List operator/(const List& other);
            bool operator|(const List& other);
            List operator<<(const List& other);
            List operator>>(const List& other);
            List operator+=(const List& other);
            List operator-=(const List& other);
            List operator*=(const List& other);
            List operator/=(const List& other);
            void operator<<=(const List& other);
            void operator>>=(const List& other);
            Bitset operator[](const int& index);
            Bitset operator[](const Generator& range);
            void operator~();

            unsigned int size() const;
            void append(const Bitset& value);
            void clear();
            void copy();
            void count(const Bitset& value);
            void extend(const Bitset& iterable);
            void index(const Bitset& value, const int start, const int end);
            void insert(const int index, const Bitset& value);
            void pop(const int index);
            void remove(const Bitset& value);
            void reverse();
            void sort();
    };

    class Tuple : public Bitset {};

    /// @brief Represents a single Pythonic variable 
    /// that can be dynamically typed and garbage collected.
    class PyObject {};
};