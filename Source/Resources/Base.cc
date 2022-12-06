/* Mamba Python by Vladyslav Korol | Apache License 2
 * This file implements the basic data structures that take
 * care of the underlying dynamic nature of Python, as well 
 * as the built-in datatypes: list, dict, set, etc. 
 * Thanks goes to @Paige Ruten (https://stackoverflow.com/users/813/paige-ruten) for the help:
   https://stackoverflow.com/questions/47981/how-do-i-set-clear-and-toggle-a-single-bit */

#include <iostream>
#include <string>
#include "Region.hh"
#include "Base.hh"

namespace mamba {
    /// @brief The helper function to convert any input data into a sequence of bytes.
    /// @tparam T The type of the input.
    /// @param value The value to cast.
    /// @return The reinterpreted byte.
    template<typename T>
    byte cast(T value) {
        return reinterpret_cast<byte>(value);
    }

    /// @brief Mamboic iterator type used to enumerate through collections. 
    /// @tparam T The type of data to be iterated through: fluid (Bitset),
    /// list (List), dictionary (Dictionary), etc.
    template<typename T> class Iterator {
        T* data;
    public:
        Iterator(T* data) {
            this->data = data;
        }
        T& operator*() {
            return *data; //Dereferenced pointer
        }
        T* operator->() {
            return data; //Pointer to the object
        }
        Iterator& operator++() {

        }
        bool operator==(const Iterator& other) {
            return data == other.data;
        }
        bool operator!=(const Iterator& other) {
            return data != other.data;
        }
    };

    /// @brief The implementation of the bitset data structure.
    /// It is used to represent dynamically typed data as a single object.
    class Bitset {
        byte* source; //The pointer to the byte source of the bitset.
        int dimensions[5]; 
        byte cache[32]; // 32 * 32 * 2 = 2048 bits
        unsigned int secondary{}; // Is used for dynamic memory allocation
        mamba::Region* storage; // The storage for the dynamic memory allocation

        /// @brief Toggles the bit at the given index with bit operators.
        /// @param bit The index of the bit to toggle.
        /// @note This method uses the bitwise XOR operator.
        /// @author Paige Ruten at StackOverflow 
        void toggle(unsigned int bit) {
            *source ^= 1 << bit;
        }
        /// @brief Sets the bit at the given index to 0.
        /// @param bit The index of the bit to set.
        /// @note This method uses the bitwise OR (|) operator.
        /// @author Paige Ruten at StackOverflow 
        void set(unsigned int bit) {
            *source |= (1 << bit);
        }
        /// @brief Sets the bit at the given index to 0.
        /// @param bit The index of the bit to clear.
        /// @note This method uses the bitwise AND (&) operator.
        /// @author Paige Ruten at StackOverflow 
        void clear(unsigned int bit) {
            *source &= ~(1 << bit);
        }
        /// @brief Checks if the bit at the given index is set.
        /// @param bit The index of the bit to check.
        /// @author Paige Ruten at StackOverflow 
        /// @return True if the bit is set, false otherwise.
        bool peek(unsigned int bit) {
            return (*source >> bit) & 1U;
        }
    public:
        /// @brief Constructs a new fresh bitset.
        /// @param pool The pointer to the memory pool 
        /// to use if the bitset size exceeds stack.
        Bitset(mamba::Region* pool) {
            storage = pool;
            source = cache;
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
                source = static_cast<byte*>(&cache);
            }
        }
        ~Bitset() {
            if (storage != nullptr) *storage >> secondary;
        }

        Iterator<Bitset> begin() {
            return Iterator<Bitset>(this);
        }

        Iterator<Bitset> end() {
            return Iterator<Bitset>(this + sizeof(Bitset));
        }

        /// @brief Returns the signed int equivalent of the bitset.
        signed int toInt() {
            return *reinterpret_cast<signed int*>(source);
        }
        /// @brief Casts the bitset into floating-point number.
        double toFloat() {
            return *reinterpret_cast<double*>(source);
        }
        /// @brief Casts the bitset into a character.
        char toChar() { }
        /// @brief Gets the integer representation of the bitset. 
        /// Do not use this function to cast to char, use toChar() instead.
        /// @tparam TIntegral The integral type of the result.
        /// @return A value of type TIntegral, which can vary from uint8_t to uint64_t.
        template<typename TIntegral> TIntegral toInt() {
            return *reinterpret_cast<TIntegral*>(source);
        }
        /// @brief Gets the size of the bytes the bitset occupies.
        unsigned int size() {
            return sizeof(*source);
        }
    };

    class String : public Bitset {
    public:
        String(const char* value);
        String(const std::string& value);
        String(const String& other);
        String(String&& other);
        String& operator=(const String& other);
        String& operator=(String&& other);
        ~String();

    };

};