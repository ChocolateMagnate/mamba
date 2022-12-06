/* Mamba Python by Vladyslav Korol | Apache License 2
 * This file implements the stack memory management in 
 * this Pythonic runtime. The stack memory is used for
 * local short-living variables and is highly optimised
 * for speed access, allocation time and cache locality. */
#include <stdexcept>
#include "Stack.hh"

namespace mamba {
    /// @brief This variable identifies the size of the frame to allocate at once.
    /// As soon as it exceeds, memory resources should go to Region.
    const unsigned int SFCapacity = 1024U;

    /// @brief One invididual cell of the stack that holds scoped data.
    /// @tparam capacity The amount of bytes the stack frame can hold.
    template<unsigned int capacity> class StackFrame {
        byte* counter;
        unsigned int size;
        byte frame[capacity];
    public:
        StackFrame() : counter{};
        /// @brief Stack allocation with the the initial value to fill.
        /// @param value 
        StackFrame(const mamba::Bitset& value) {
            for (unsigned int i = 0; i < capacity; i++) frame[i] = value[i];
        }

        /// @brief Stack frame destructor that will release all resources.
        ~StackFrame();

        /// @brief The [] operator overloading to read a value from the stack.
        /// @param index The integer position of the value in the frame beginning from zero.
        /// @return The bytes that correspond to the given index.
        byte* operator[](unsigned int index);
        /// @brief Pushes a new value on the top of the stack.
        /// @param value The value to push: Bitset, List, Dictionary, etc.
        void push(const byte* value, const unsigned int size) {
            if (this->counter <= this->capacity)
                throw std::runtime_error("StackFrameOverflow");
            for (unsigned int _byte = 0; _byte < size; _byte++) {
                this->frame[this->counter] = value[_byte];
                this->counter++;
            }
            ++size;
        }
        /// @brief Removes the top value from the stack and returns it.
        /// @param amount The amount of bytes to pop from the stack. They must
        /// represent a single value, otherwise the stack might be corrupted.
        byte* pop(unsigned int amount) {
            if (this->counter < amount)
                throw std::runtime_error("StackFrameUnderflow");
            this->counter -= amount;
            --size;
            return this->frame[this->counter];
        }
        /// @brief Empties the entire stack frame.
        void clear();

        /// @brief Gets the size of all values living in the stack frame.
        unsigned int size() const {
            return this->size;
        }
        /// @brief Gets the maximum capacity of the stack frame supplied during initialisation.
        unsigned int capacity() const {
            return this->capacity;
        }
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
        mamba::Iterator<mamba::StackFrame<200>> begin();
        /// @brief Returns the iterator (pointer wrapper) to one position past
        /// the stack end. It is used to enumerate through it.
        mamba::Iterator<mamba::StackFrame<200>> end();
        
        /// @brief Gets the total size of all active values on the stack.
        unsigned int size() const;
};
};