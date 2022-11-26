#pragma once
#include "Base.hh"
namespace mamba {
    /// @brief The implementation of the Mamba memory pool
    /// used to store all varying-sized data.
    template<unsigned int size>
    class Region {
        public:
            //Resource Acquisition Is Initialisation
            Region(unsigned int size); //The default constructor that allocates the memory pool.
            Region(const Region& other); //Produces a new region with the other one appended.
            Region(const Region&& other); //Moves the memory pool to the new region.
            Region& operator=(const Region& other); 
            Region& operator=(const Region&& other);
            ~Region(); //Releases all resources.

            //Resource operator managament
            template<typename T> void operator<<(const T& other);
            template<typename T> T* operator[](unsigned int index);
            mamba::Bitset& operator[](unsigned int index);
            

    };
};