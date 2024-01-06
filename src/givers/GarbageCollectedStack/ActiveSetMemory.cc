#include "ActiveSetMemory.hh"

#include <cmath>
#include <cstring>

#include "context.hh"
namespace mamba {
  ActiveSetMemory::ActiveSetMemory() noexcept {
    try {
      capacity = DefaultStackSize;
      pool = std::make_unique<std::byte[]>(DefaultStackSize);
      topOfStack = pool.get();
      initialisePages();
    } catch (std::bad_alloc&) {
      raise(Signal::MemoryError, ExceptionReason::HostDoesNotHaveEnoughMemoryToStart);
    }
  }

  const MemoryUsageStatistics& ActiveSetMemory::getMemoryUsage() const noexcept {
    return statistics;
  }

  size_t ActiveSetMemory::occupiedBytes() const noexcept {
    return statistics.usedMemorySize;
  }

  size_t ActiveSetMemory::freeBytes() const noexcept {
    return statistics.allocatedMemorySize - statistics.usedMemorySize;
  }

  size_t ActiveSetMemory::getBookmarks() const noexcept { return statistics.allocatedObjects; }

  std::byte* ActiveSetMemory::top() const noexcept {
    return topOfStack;
  }

  unsigned int ActiveSetMemory::getGrowthFactor() const noexcept {
    return growthFactor;
  }

  void ActiveSetMemory::setGrowthFactor(const unsigned int factor) noexcept {
    if (factor != 0) growthFactor = factor;
  }

  unsigned int ActiveSetMemory::getPreservationFactor() const noexcept {
    return preservationFactor;
  }

  void ActiveSetMemory::setPreservationFactor(const unsigned int factor) noexcept {
    if (factor < 101) preservationFactor = factor;
  }

  void ActiveSetMemory::push() {
    frames.push_back(topOfStack);
  }

  void ActiveSetMemory::pop() {
    const ptrdiff_t reclaimedMemory = topOfStack - frames.back();
    topOfStack = frames.back();
    frames.pop_back();
    statistics.usedMemorySize -= reclaimedMemory;
  }

  std::byte* ActiveSetMemory::gather(const size_t bytesToAllocate) noexcept {
    if (topOfStack + bytesToAllocate > pool.get() + capacity) return nullptr;
    std::byte* destination = topOfStack;
    topOfStack += bytesToAllocate;
    statistics.usedMemorySize += bytesToAllocate;
    return destination;
  }

  std::byte* ActiveSetMemory::copy(const void* original, const size_t size) noexcept {
    std::byte* destination = gather(size);
    (void)std::memcpy(destination, original, size);
    return destination;
  }

  void ActiveSetMemory::mark(const std::byte* destination, const size_t size) noexcept {
    const ptrdiff_t slabIndex = (destination - pool.get()) / SlabSize;
    const unsigned int pageIndex = std::ceil(slabIndex / PageSize);
    const int slabAmount = std::ceil(size / SlabSize);
    if (pageIndex <= pages.size()) {
      pages[pageIndex].markAsGarbage(destination, slabAmount);
      statistics.garbageMemorySize += size;
    }
  }

  void ActiveSetMemory::grow(const size_t moreBytes) noexcept {
    if (growthFactor == 1 && moreBytes == 0) return;
    try {
      const size_t normalGrowth = statistics.allocatedMemorySize * growthFactor;
      const size_t newSizeInBytes = std::max(statistics.allocatedMemorySize + moreBytes, normalGrowth);
      resize(newSizeInBytes);
      ++statistics.growths;
    } catch (std::bad_alloc&) { raise(Signal::MemoryError, ExceptionReason::HostRanOutOfMemory); }
  }

  ActiveSetMemory& ActiveSetMemory::reserve(size_t reservedSizeInSlots) noexcept {
    if (freeBytes() < reservedSizeInSlots) grow(reservedSizeInSlots);
    return *this;
  }

  bool ActiveSetMemory::shrink() noexcept {
    if (preservationFactor == 100) return false;
    const size_t preservedMemorySize = preservationFactor / 100 * statistics.garbageMemorySize;
    const size_t reclaimedMemorySize = statistics.garbageMemorySize - preservedMemorySize;
    const size_t reducedPoolSize = statistics.allocatedMemorySize - statistics.garbageMemorySize
                                   + preservedMemorySize;
    const bool reducedPoolWillBeTooSmall = reclaimedMemorySize <= PageSize;
    if (reducedPoolWillBeTooSmall) return false;
    try {
      resize(reducedPoolSize);
      ++statistics.shrinks;
      return true;
    } catch (std::bad_alloc&) {
      raise(Signal::MemoryError, ExceptionReason::HostRanOutOfMemory);
      return false;
    }
  }

  void ActiveSetMemory::resize(const size_t newSize) noexcept {
    size_t counter = 0, reclaimedGarbageSize = 0;
    std::unique_ptr<std::byte[]> resizedPool = std::make_unique<std::byte[]>(newSize);
    for (GarbageBitsetPage& page : pages) {
      const GarbageCollected<> aliveSegment = page.yieldNextAliveSector();
      if (aliveSegment.capacity + counter > newSize) break;
      (void)std::memcpy(resizedPool.get() + counter, aliveSegment.destination, aliveSegment.capacity);
      counter += aliveSegment.capacity;
      reclaimedGarbageSize += page.getGarbageSizeBefore(aliveSegment.destination);
    }
    topOfStack = resizedPool.get() + counter;
    statistics.allocatedMemorySize = newSize;
    statistics.usedMemorySize = counter;
    pool = std::move(resizedPool);
    initialisePages();
  }

  void ActiveSetMemory::initialisePages() {
    const size_t numberOfPages = capacity / PageSize;
    pages.clear();
    pages.reserve(numberOfPages);
    for (size_t pageIndex = 0; pageIndex < numberOfPages; ++pageIndex)
      pages[pageIndex] = GarbageBitsetPage(pool.get() + pageIndex * PageSize);
  }

  void ActiveSetMemory::clear() {
    growthFactor = InitialGrowthFactor;
    preservationFactor = InitialPreservationFactor;
    statistics = {0, 0, DefaultStackSize, 0, 0, 0 };
    std::unique_ptr<std::byte[]> clearedSlots = std::make_unique<std::byte[]>(DefaultStackSize);
    pool = std::move(clearedSlots);
    topOfStack = pool.get();
  }

}