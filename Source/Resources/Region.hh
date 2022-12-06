#pragma once
#include "Base.hh"
#define byte unsigned char // 8-bit unsigned integer
namespace mamba {
    /// @brief The implementation of the Mamba memory pool
    /// used to store all varying-sized data.

    class Region {
        public:
            //Resource Acquisition Is Initialisation
            template<unsigned int size>
            Region(); //The default constructor that allocates the memory pool.
            Region(const Region& other); //Produces a new region with the other one appended.
            Region(const Region&& other); //Moves the memory pool to the new region.
            Region& operator=(const Region& other); 
            Region& operator=(const Region&& other);
            ~Region(); //Releases all resources.

            /// @brief Appends a piece of data on the managed memory pool.
            /// @tparam TBlock The type of the data to fill.
            /// @param other The piece of data to append.
            /// @return The position of the block in order that can be
            /// accessed with the [] operator.
            template<typename TBlock> unsigned int operator<<(const TBlock& other);
            /// @brief Releases the segment of the memory pool.
            /// @param position The index of the item in the memory pool.
            /// @note This method does not require the type as it can be deduced.
            /// @return True if deallocation succeeded, false otherwise.
            bool operator>>(const unsigned int position);
            /// @brief Prodives the access to items in the memory pool.
            /// @tparam TBlock The type of the data to access.
            /// @param index The positional number in the region.
            /// @note This method does not perform type or bounds checking.
            /// @return The pointer to the item that can be used to read or write value.
            template<typename TBlock> TBlock* operator[](unsigned int index);
            /// @brief The default accessor for the region that does
            /// not require type specification.
            /// @param index The position of the item in the region.
            /// @return The pointer to the bytes stored in the region.
            byte* operator[](unsigned int index);

            /// @brief The exposition getter for the allocated block.
            /// @return The pointer to the bytes residing in the region.
            byte* data();

            

    };
};