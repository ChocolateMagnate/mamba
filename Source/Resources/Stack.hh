/* Mamba Python by Vladyslav Korol | Apache License 2
 * This file is devoted to illustrate automatic resource management
 * using the stack memory. It would lead to significantly greater
 * cache locality and be less fragmentation-prone. */
#pragma once
#include "Base.hh"
#define byte unsigned char
namespace mamba {
    /// @brief One invididual cell of the stack that holds scoped data.
    /// @tparam capacity The amount of bytes the stack frame can hold.
    template<unsigned int capacity> class StackFrame {
        public:
            StackFrame();
            /// @brief Stack allocation with the the initial value to fill.
            /// @param value 
            StackFrame(const mamba::Bitset& value);
            /// @brief Stack frame destructor that will release all resources.
            ~StackFrame();

            /// @brief The [] operator overloading to read a value from the stack.
            /// @param index The integer position of the value in the frame beginning from zero.
            /// @return 
            mamba::Bitset& operator[](unsigned int index);
            /// @brief Pushes a new value on the top of the stack.
            /// @param value The value to push: Bitset, List, Dictionary, etc.
            void push(const mamba::Bitset& value);
            /// @brief Removes the top value from the stack and returns it.
            mamba::Bitset pop();
            /// @brief Empties the entire stack frame.
            void clear();

            /// @brief Gets the size of all values living in the stack frame.
            unsigned int size() const;
            /// @brief Gets the maximum capacity of the stack frame supplied during initialisation.
            unsigned int capacity() const;
    };

    class Stack {
        public:
            Stack();
            ~Stack();

            /// @brief Overloads stack access operation.
            /// @param index The index of the stack value to access.
            /// @return A value living at the stack at the given position.
            mamba::Bitset operator[](unsigned int index);

            /// @brief Returns the iterator (pointer wrapper) to the
            /// beginning of the stack. It is used to enumerate through it.
            mamba::Iterator<mamba::StackFrame> begin();
            /// @brief Returns the iterator (pointer wrapper) to one position past
            /// the stack end. It is used to enumerate through it.
            mamba::Iterator<mamba::StackFrame> end();
            
            /// @brief Gets the total size of all active values on the stack.
            unsigned int size() const;
    };
};