
#include <string.h> //to use memcpy
#include "Base.hh"
#include "Region.hh"

namespace mamba {
    /// @brief Implementation of the Mamba memory pool.
    template<unsigned int size>
    class Region {
        void* bytes;
        unsigned int count, current;
        mamba::Bitset* items[size]; //Pointer storage.
        public:
            //Generates a new region of the given size.
            Region(unsigned int size = 1024) {
                this->size = size;
                bytes = new char[size];
            }
            //Produces a new region with the other one appended.
            Region(const Region& other) {
                void* extension = new char[this->size + other.size];
                memcpy(extension, bytes, size);
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
            /// @return The void pointer to the item.
            mamba::Bitset& operator[](unsigned int index) {
                return *items[index];
            }
            /// @brief Looks for the free space in the region.
            /// @tparam T The type of the value to be stored.
            /// @param other The value to be stored.
            /// @return The integer of the available space.
            template<typename T> void operator<<(const T& other) {
                memccpy(bytes + count, &other, sizeof(T));
                items[current] = (mamba::Bitset*)(bytes + count);
                count += sizeof(T);
                current++;
            }
    };
};