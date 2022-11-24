
#include <string.h> //to use memcpy
#include "Region.hh"

namespace mamba {
    /// @brief Implementation of the Mamba memory pool.
    template<typename Contained>
    class Region {
        void* bytes;
        unsigned int size, count, step;
        public:
            //Generates a new region of the given size.
            Region(unsigned int size = 1024) {
                this->size = size;
                this->step = sizeof(Contained);
                bytes = new unsigned int[size];
            }
            //Produces a new region with the other one appended.
            Region(const Region& other) {
                void* extension = new unsigned int[this->size + other.size];
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
                return (TReturn*)bytes + index;
            }
            /// @brief Provides access to the element in the region.
            /// @param index The number of the item from the beginning.
            /// @return The void pointer to the item.
            void* operator[](unsigned int index) {
                return bytes + index * step;
            }
    };
};