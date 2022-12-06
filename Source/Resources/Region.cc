/* Mamba Python by Vladyslav Korol | Apache License 2
 * This file implements the memory pool model used to
 * store all heap-allocated body the first time such
 * an immense value is required. It operates as blocks
 * of bytes as aliased unsigned chars. */

#include <stdexcept> // std::out_of_range
#include <string.h> // memcpy
#include "Base.hh" // Todo: measure the size of types and substitude this header with constants.
#include "Region.hh"

namespace mamba {
    /// @brief Implementation of the Mamba memory pool.
    /// @note This class consists of two parts: head and body. The body is
    /// the continuous storage where all the data lives, meanwhile the head
    /// is the array of byte* that point to each individual value.  
    class Region {
        byte *bytes, *items; //The pointers to the beginning and segmented values in the memory pool.
        unsigned int capacity, count, index, neck; 
    public:
        /// @brief Generates a new region of the given size.
        /// @tparam size The maximum count of items the memory pool can hold.
        /// @param size The number of bytes to allocate on the heap.
        template<unsigned int capacity> Region(unsigned int size = 1024) {
            this->capacity = capacity;
            //The separation point between the head and the body.
            this->neck = sizeof(byte*) * capacity; 
            this->count = this->neck + 1;
            bytes = new byte[size + neck];
        }
        //Produces a new region with the other one appended.
        Region(const Region& other) {
            byte* extension = new byte[this->capacity + this->neck + other.capacity + other.neck];
            memcpy(extension, this->bytes, this->capacity);
            delete[] bytes;
            bytes = extension;
        }

        ~Region() {
            delete[] bytes;
        }
        /// @brief Provides access to the element in the region.
        /// @tparam TReturn The type of the value to be accessed.
        /// @param index The index of the element to be accessed.
        /// @return The pointer of the type TReturn to the element.
        template<typename TReturn> TReturn* operator[](unsigned int index) {
            return (TReturn*)*items[index];
        }
        /// @brief Provides access to the element in the region.
        /// @param index The number of the item from the beginning.
        /// @return The byte pointer to the item.
        byte* operator[](unsigned int index) {
            return &items[index];
        }
        /// @brief Looks for the free space in the region.
        /// @tparam T The type of the value to be stored.
        /// @param other The value to be stored.
        /// @return The integer of the available space.
        template<typename T> void operator<<(const T& other) {
            const int sizeT = sizeof(T);
            if (sizeT > this->capacity) throw std::out_of_range
                ("The region {}-byte-wide is too small to store the value.", capacity);
            memccpy(bytes + count, other, sizeT);
            body[current] = (mamba::Bitset*)(bytes + count);
            count += sizeof(T);
            index++;
        }
        /// @brief Releases the memory from the region.
        /// @param position The item position in the region.
        /// If out of the bounds, an exception is thrown.
        /// @note This method doesn't excplicitly frees the memory,
        /// but only decreases the counters so that the next time
        /// memory is required, it is going to be overwritten.
        void operator>>(const unsigned int position) {
            if (position >= capacity) throw std::out_of_range
                ("The position is out of the bounds in block of {}.", capacity);
            count -= sizeof(items[position]);
            index--;
        }

        /// @brief The exposition getter for the allocated block.
        /// @return The pointer to the bytes residing in the region.
        byte* data() {
            return bytes;
        }
    };
};