#pragma once
#include <string>
namespace mamba {

    class Algorithm {

    };
    /// @brief Describes Pythonic lazy generators and iterators.
    class Generator {
        public:
            Generator(Algorithm& function);
            Bitset generate(); //Yields the next value.
    };
    /// @brief The basic bitset used to represent fluid data.
    class Bitset {
        public:
            Bitset(); //Default constructor.
            template<int size> Bitset(); //Constructor for a bitset of a given size.
            template<int size> Bitset(int* state); //Constructor for a bitset of a given size and initial value.
            Bitset(const Bitset& other); //Copy constructor.
            Bitset(Bitset&& other); //Move constructor.
            Bitset& operator=(const Bitset& other); //Copy assignment operator.
            Bitset& operator=(Bitset&& other); //Move assignment operator.
            ~Bitset(); //Destructor.

            Bitset operator+(const Bitset& other);
            Bitset operator-(const Bitset& other);
            Bitset operator*(const Bitset& other);
            Bitset operator/(const Bitset& other);
            Bitset operator%(const Bitset& other);
            Bitset operator^(const Bitset& other);
            void operator&(const Bitset& other);
            bool operator|(const Bitset& other);
            Bitset operator<<(const Bitset& other);
            Bitset operator>>(const Bitset& other);
            Bitset operator+=(const Bitset& other);
            Bitset operator-=(const Bitset& other);
            Bitset operator*=(const Bitset& other);
            Bitset operator/=(const Bitset& other);
            Bitset operator%=(const Bitset& other);
            Bitset operator^=(const Bitset& other);
            void operator&=(const Bitset& other);
            void operator|=(const Bitset& other);
            void operator<<=(const Bitset& other);
            void operator>>=(const Bitset& other);
            bool operator==(const Bitset& other);
            bool operator!=(const Bitset& other);
            bool operator<(const Bitset& other);
            bool operator>(const Bitset& other);
            void operator<=(const Bitset& other);
            void operator>=(const Bitset& other);
            int operator[](const Bitset& other);
            int operator[](const int& other);
            void operator~();

            unsigned int size() const; //Returns the size of the bitset.
            //To be continued...

    };

    class Float : public Bitset {};

    class String : public Bitset {
        public:
            String();
            String(const char* value);
            String(const std::string& value);
            String(const String& other);
            String(const String&& other);
            String(const Bitset& other); //String conversion constructor.
            String& operator=(const String& other);
            String& operator=(String&& other);
            ~String();

            String operator+(const String& other);
            String operator-(const String& other);
            String operator*(const String& other);
            String operator/(const String& other);
            bool operator|(const String& other);
            String operator<<(const String& other);
            String operator>>(const String& other);
            String operator+=(const String& other);
            String operator-=(const String& other);
            String operator*=(const String& other);
            String operator/=(const String& other);
            void operator<<=(const String& other);
            void operator>>=(const String& other);
            bool operator==(const String& other);
            bool operator!=(const String& other);
            bool operator<(const String& other);
            bool operator>(const String& other);
            void operator<=(const String& other);
            void operator>=(const String& other);
            char operator[](const int& other);
            char operator[](const Generator& range);
            void operator~();

            unsigned int size() const;
            Bitset split(const String* delimiters);
            Bitset split(const char** delimiters);
            void capitalize();
            void casefold();
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
            void get(const Bitset& key, const Bitset& default);
            void pop(const Bitset& key);
            void popitem();
            void setdefault(const Bitset& key, const Bitset& default);
            void update(const Bitset& other);
            void update(const Bitset& other, const Bitset& other2);

    };
};