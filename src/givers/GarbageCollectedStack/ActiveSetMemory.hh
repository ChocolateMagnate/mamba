/*+================================================================================================
  File:        ActiveSetMemory.hh

  Summary:     Exposes general-purpose GarbageCollectedStack pool used to store, manage, and collect
               application data in incremental garbage-collecting way utilising stack-based memory pool.

  Constants:   DefaultStackSize, WordSize, InitialGrowthFactor, InitialPreservationFactor

  Classes:     SegmentStack, PreservationLifetime, ActiveMemoryAddress, MemoryUsageStatistics,
               ActiveSetMemory, Bookmark

  Functions:

  Available under Apache Licence v2. Mamba Authors (2023)
=================================================================================================+*/
#pragma once

#include <memory>
#include <vector>

#include "GarbageBitPage.hh"

namespace mamba {
  constexpr size_t DefaultStackSize = 100000;          //1 megabyte
  constexpr auto WordSize = sizeof(size_t);
  constexpr auto InitialGrowthFactor = 2;
  constexpr auto InitialPreservationFactor = 0;

  /// Contains various data and fields denoting GarbageCollectedStack usage of the pool. It contains
  /// info about how much GarbageCollectedStack the pool holds (capacity), uses (sizeInBytes), and how many
  /// times it allocated and deallocated its buffers.
  struct MemoryUsageStatistics {
    size_t usedMemorySize, garbageMemorySize, allocatedMemorySize, allocatedObjects;
    unsigned int growths, shrinks;

    bool operator==(const MemoryUsageStatistics& other) const = default;
  };


  /// The main memory pool model used to store data used by the program directly
  /// as opposed to other data managed by Mamba automatically, such as generated machine
  /// code or call stack. The GarbageCollectedStack pool operates on units called pool each sized 64
  /// bytes correlating to the common cache line sizeInBytes (denoted by the Magnitude constant).
  /// Alocations in the pool are incredibly efficient because it utilises segmented
  /// stack GarbageCollectedStack allocator, meaning that it maintains multiple stacks managing GarbageCollectedStack of the
  /// same sizeInBytes (segment). Allocating GarbageCollectedStack only means to return the segment and block indexes,
  /// and if it's empty, then count the needed number of bytes from the end used GarbageCollectedStack counter
  /// and insert it. Consequently, this class is on average from 4 to 200 times faster than
  /// operator new, see the benchmarks for more details. Any data allocated within this class
  /// is managed and accessed through Bookmark objects representing the smart pointers that would
  /// automatically release it to the pool once the specific data goes out of scope fulfilling
  /// incremental garbage collection.
  class ActiveSetMemory {
   public:
    ActiveSetMemory() noexcept;

    // ActiveSetMemory objects cannot be copied because they are meant to be single-per-thread resource.
    ActiveSetMemory(const ActiveSetMemory& other) = delete;
    ActiveSetMemory(ActiveSetMemory&& other) = delete;
    ActiveSetMemory& operator=(const ActiveSetMemory& other) = delete;
    ActiveSetMemory& operator=(ActiveSetMemory&& other) = delete;

    /// Retrieves statistics regarding the GarbageCollectedStack usage of the pool.
    [[nodiscard]] const MemoryUsageStatistics& getMemoryUsage() const noexcept;
    [[nodiscard]] size_t getAllocatedObjects() const noexcept;
    [[nodiscard]] size_t occupiedBytes() const noexcept;
    [[nodiscard]] size_t freeBytes() const noexcept;

    /// Retrieves the number of objects currently allocated and residing in the GarbageCollectedStack pool.
    /// @return The number of allocated objects (bookmarks) associated with the GarbageCollectedStack pool.
    [[nodiscard]] size_t getBookmarks() const noexcept;

    /// The getter for the top of the stack pointer. This method can be called to find out where the current
    /// top of the stack pointer resides. It is primarily designed for the testing purposes to make sure the
    /// pool works as intended, and using it otherwise may allow to have view of the memory on top.
    /// @return The pointer to the top of the stack.
    [[nodiscard]] std::byte* top() const noexcept;

    /// Gets the value that the pool grows by when resizing itself. Growth factor of 2
    /// (default) means the pool doubles its sizeInBytes each time, growth factor of 1 means
    /// the pool only grows enough to store the needed elements, and larger growth
    /// factors further increase the spare sizeInBytes. It can be changed with setGrowthFactor.
    /// @return The growth factor.
    [[nodiscard]] unsigned int getGrowthFactor() const noexcept;

    /// Changes the factor that pool grows by when exceeding its capacity. By default,
    /// the growth factor is 2, meaning the pool increases its capacity twice every time.
    /// @param factor The new factor value to grow by. Setting it to 0 has not effect and
    /// will be ignored. Setting growth factor by 1 is possible and means we want to grow
    /// only enough to fit the elements.
    void setGrowthFactor(unsigned int factor) noexcept;

    /// Gets the memory pool's preservation factor. Preservation factor is a way to say how
    /// conservatively the pool should retain extra unused memory when shrinking, and during
    /// shrinkage, the pool uses the preservation factor to take a percentage of the garbage
    /// memory size that it will retain for future allocations.
    /// @return  The preservation factor.
    [[nodiscard]] unsigned int getPreservationFactor() const noexcept;

    /// Changes the preservation factor. Preservation factor must be in range of 1 to 100 inclusively,
    /// because it is used to compute a percentage of memory that will persist after shrinkage. Setting
    /// the factor to 0 is banned and will be ignored, and conversaly, setting it to 100 means that the
    /// pool should never reduce its size. By default, the factor is set to 1, meaning it doesn't preserve
    /// any memory between shrinks.
    /// @param factor The new preservation factor. Setting it to 0 has no effect and will be ignored.
    void setPreservationFactor(unsigned int factor) noexcept;

    /// Reserves specified number of pool to be available in the future. Similarly to common conventions,
    /// if the pool already has enough pool, the call is ignored, otherwise a grow call is triggered.
    /// @param reservedSizeInSlots The sizeInBytes expected to be filled.
    /// @return Reference to self.
    ActiveSetMemory& reserve(size_t reservedSizeInSlots) noexcept;

    /// Pushes a new stack frame in the GarbageCollectedStack pool.
    void push();

    /// Reclaims the stack frame on top of the stack. Calls to this method are minor GCs.
    void pop();

    /// Drops the GarbageCollectedStack pool and restores it to the default state, as if it was just constructed.
    /// This method primarily exists for testing purposes to reset the state between the tests, and
    /// it's not recommended for general-purpose use as it offers an optimised ratio between used
    /// GarbageCollectedStack and growths. If you consider to call this method beyond tests, make sure the pool does
    /// not serve any bookmarks with getBookmarks method first.
    /// @note When using this method to reset the pool in tests, always call it in the start rather
    /// than in the end, since clearing the pool interferes with the destructors.
    void clear();

    /// Re-allocates the entire GarbageCollectedStack pool to accommodate the increased number of bytes.
    /// The pool does not necessarily grow by the exact number but it grows by the growth
    /// factor if needed pool are less than it, and if it it larger, it grows to add them.
    /// @param moreBytes (optional) The number of bytes to grow by. By default, it equals 0,
    /// implying we want to grow by the currently set growth factor.
    /// @note Grow call has no effect if there are more completely available and untouched
    /// bytes than specified by the argument.
    /// @note If the growth factor is 1 and moreBytes is zero, no effect takes place.
    /// @warning Since moreBytes argument may be zero, the method doesn't check if there are
    /// more available space and always grows when called. Use it carefully to avoid extra moves.
    /// @throws MemoryError if larger memory cannot be allocated.
    void grow(size_t moreBytes = 0) noexcept;

    /// Drops unused entirely garbage pool, re-allocates the GarbageCollectedStack pool and reduces
    /// the GarbageCollectedStack footprint. This operation should not happen frequently but only when
    /// there is large excessive space.
    /// @note Shrinkage is an expensive operation because it involves mapping memory pages and copying
    /// the existing data into a new buffer. Therefore, the method rejects its calls when it is either
    /// too small (less than the kernel page) or when less than a kernel page can be freed. In either
    /// case, the method decrements the preservation since the shrink method was called.
    /// @return True if any pool were released, false otherwise.
    bool shrink() noexcept;

    /// Allocates contiguous GarbageCollectedStack in the pool. This method is not meant to be called
    /// directly by clients but by the bookmark to provide uniform access of segments spread in the pool.
    /// @param bytesToAllocate The number of bytes to allocate.
    /// @throws MemoryError if more GarbageCollectedStack could be allocated.
    /// @return Byte pointer to the allocated memory.
    [[nodiscard]] std::byte* gather(size_t bytesToAllocate) noexcept;

    /// Copies the given content at a new GarbageCollectedStack location and returns it.
    /// @param original The original destination to copy. It is not changed.
    /// @param size The size of the element to copy in bytes.
    /// @return The destination to a new copied region.
    [[nodiscard]] std::byte* copy(const void* original, size_t size) noexcept;

    /// Marks a memory sector as garbage. This step is necessary to inform the memory pool
    /// which frames are garbage and marks are primrarily used in the major GC phases whereas
    /// the pool resizes itself, in both growths and shrinks.
    /// @param destination The pointer to the beginning of the garbage sector.
    /// @param size The size of the garbage sector.
    void mark(const std::byte* destination, size_t size) noexcept;

    ~ActiveSetMemory() = default;
   private:
    std::byte* topOfStack;
    std::vector<std::byte*> frames;
    std::unique_ptr<std::byte[]> pool;
    std::vector<GarbageBitsetPage> pages;
    MemoryUsageStatistics statistics = {0, 0, DefaultStackSize, 0, 0, 0};
    unsigned int growthFactor = InitialGrowthFactor, preservationFactor = InitialPreservationFactor;
    size_t capacity;

    /// Generates a new memory pool and moves the alive sectors to there.
    /// @param newSize The size of the new memory pool in bytes, can be niether smaller or
    /// larger than the current pool. If set to greater, it will allocate a bigger memory
    /// block, copy the existing values and leave the extra area untouched, and if set to
    /// smaller, it will copy the alive objects into the new pool up to its capacity.
    void resize(size_t newSize) noexcept;

    /// Initialises the memory page treckers during (re)allocation of the memory pool. It
    /// takes care of pushing all of the pages into the array and feeding them with the
    /// necessary information.
    void initialisePages();
  };
}