/* Mamba Python by Vladyslav Korol | Apache License 2
 * This file implements the basic data structures that take
 * care of the underlying dynamic nature of Python, as well 
 * as the built-in datatypes: list, dict, set, etc. */

#include <iostream>
#include <string>

#include "Base.hh"

namespace mamba {
    class Bitset {
        int dimensions[5]; 
        unsigned long long int main[32]; // 32 * 32 * 2 = 2048 bits
        unsigned long long int* secondary = nullptr; // Is used for dynamic memory allocation
        public:
            Bitset() {
                for (int index = 0; index < 32; ++index) {
                    main[index] = 0;
                    if (index < 5) dimensions[index] = 0;
                }
            }
            template<int size> Bitset() {
                if (size > 2048) secondary = new unsigned long long int[size];
            }
    };

    class String : public Bitset {
        public:
            String() {

            }
            String(const char* value);
            String(const std::string& value);
            String(const String& other);
            String(String&& other);
            String& operator=(const String& other);
            String& operator=(String&& other);
            ~String();

            String operator+(const String& other) {
                return String();
            }
            String operator-(const String& other);
            String operator*(const String& other);
            String operator/(const String& other);
            String operator%(const String& other);
            String operator^(const String& other);
            void operator&(const String& other);
            bool operator|(const String& other);
            String operator<<(const String& other);
            String operator>>(const String& other);
            String operator+=(const String& other);
            String operator-=(const String& other);
            String operator*=(const String& other);
            String operator/=(const String& other);
            String operator%=(const String& other);
            String operator^=(const String& other);
            void operator&=(const String& other);
            void operator|=(const String& other);
            void operator<<=(const String& other);
            void operator>>=(const String& other);
            bool operator==(const String& other);
            bool operator!=(const String& other);
            bool operator<(const String& other);
            bool operator>(const String& other);
            void operator<=(const String& other);
            void operator>=(const String& other);
            int operator[](const String& other);
            int operator[](const int& other);
            void operator~();

            unsigned int size() const;
            const char* toConstChar() const;
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
    };

};