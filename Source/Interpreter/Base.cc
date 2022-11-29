/* Mamba Python by Vladyslav Korol | Apache License 2
 * This file implements the basic data structures that take
 * care of the underlying dynamic nature of Python, as well 
 * as the built-in datatypes: list, dict, set, etc. */

#include <iostream>
#include <string>
#include "Base.hh"
#include "Region.hh"

namespace mamba {


    /// @brief The implementation of the bitset data structure.
    /// It is used to represent dynamically typed data as a single object.
    class Bitset {
        byte* source;
        int dimensions[5]; 
        byte main[32]; // 32 * 32 * 2 = 2048 bits
        unsigned int secondary{}; // Is used for dynamic memory allocation
        mamba::Region* storage; // The storage for the dynamic memory allocation

        /// @brief Toggles the bit at the given index with bit operators.
        /// @param bit The index of the bit to toggle.
        /// @note This method uses the bitwise OR (|) operator.
        /// @author Paige Ruten at StackOverflow (link in the header)
        /// @return The state of the toggled bit.
        bool toggle(unsigned int bit) {
            *source |= (1 << bit);
        }
        /// @brief Sets the bit at the given index to 0.
        /// @param index The index of the bit to set.
        /// @author Paige Ruten at StackOverflow (link in the header)
        void set(unsigned int index);
        /// @brief Sets the bit at the given index to 0.
        /// @param index The index of the bit to clear.
        /// @author Paige Ruten at StackOverflow (link in the header)
        void clear(unsigned int index);
    public:
        /// @brief Constructs a new fresh bitset.
        /// @param pool The reference to the memory pool 
        /// to use if the bitset size exceeds stack.
        Bitset(mamba::Region& pool) {
            storage = &pool;
            source = static_cast<byte*>(&main);
        }
        /// @brief Constructs a new bitset with the requirements.
        /// @tparam size The minimal required size of the bitset in bytes.
        /// @param pool The memory pool (defined in Region.hh) 
        /// where to allocate the bitset to.
        template<int size> Bitset(mamba::Region& pool) {
            if (size > 2048 / 8) {
                secondary = pool << [size];
                source = pool[secondary];
            } else {
                storage = nullptr;
                source = static_cast<byte*>(&main);
            }
        }
        ~Bitset() {
            if (storage != nullptr) *storage >> secondary;
        }

        /// @brief Returns the signed int equivalent of the bitset.
        signed int toInt() {}
        /// @brief Casts the bitset into floating-point number.
        double toFloat() {}
        /// @brief Casts the bitset into a character.
        char toChar() {}
        /// @brief Gets the integer representation of the bitset.
        /// @tparam TIntegral The integral type of the result.
        /// @return A value of type TIntegral, which can vary from uint8_t to uint64_t.
        template<typename TIntegral> TIntegral toInt() {}



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