#include "GarbageCollectedStack/ActiveSetMemory.hh"
#include "givers/multithreading/store.hh"
#include "givers/memory.hh"
namespace mamba {
  GarbageCollected<> gather(const size_t size, const GarbageCollectionGeneration lifetime) {
    GarbageCollected<> reference;
    auto& memory = select<ActiveSetMemory>();
    reference.destination = memory.gather(size);
    reference.capacity = size;
    reference.lifetime = lifetime;
    return reference;
  }

  GarbageCollected<> clone(const GarbageCollected<>& original) {
    GarbageCollected<> reference;
    reference.destination = select<ActiveSetMemory>().copy(original.destination, original.capacity);
    reference.capacity = original.capacity;
    reference.lifetime = original.lifetime;
    return reference;
  }

  GarbageCollected<> clone(const GarbageCollected<>& original, GarbageCollectionGeneration lifetime) {
    GarbageCollected<> reference;
    reference.destination = select<ActiveSetMemory>().copy(original.destination, original.capacity);
    reference.capacity = original.capacity;
    reference.lifetime = lifetime;
    return reference;
  }

  /// Finalises the act of garbage collection and gathers the information returned to the caller.
  /// @param previousStatistics The reference to the memory usage statistics before the GC.
  /// @param memory The reference to the ActiveSetMemory object where GC was done.
  /// @return The final summary of the garbage collection.
  GarbageCollectionSummary generateGarbageCollectionSummary(const MemoryUsageStatistics& previousStatistics,
                                                            const ActiveSetMemory& memory) {
    const MemoryUsageStatistics& currentStatistics = memory.getMemoryUsage();
    const size_t reclaimedMemory =
      previousStatistics.allocatedMemorySize - currentStatistics.allocatedMemorySize;
    const size_t aliveMemory = currentStatistics.usedMemorySize;
    const GarbageCollectionSummary summary = {reclaimedMemory, aliveMemory};
    return summary;
  }

  GarbageCollectionSummary collect(const GarbageCollectionGeneration level,
                                   const size_t extraAvailableMemoryNextRound) {
    auto& memory = select<ActiveSetMemory>();
    const MemoryUsageStatistics previousStatistics = memory.getMemoryUsage();

    switch (level) {
      case GarbageCollectionGeneration::Eden:
      case GarbageCollectionGeneration::Young:
      case GarbageCollectionGeneration::Elder:
        memory.pop();
      case GarbageCollectionGeneration::Antique:
      case GarbageCollectionGeneration::Pernament:
        const size_t reclaimedMemory = previousStatistics.allocatedMemorySize -
                                       previousStatistics.garbageMemorySize;
        const size_t availableMemoryAfterGarbageCollection = reclaimedMemory +
                                       previousStatistics.usedMemorySize;
      if (availableMemoryAfterGarbageCollection > extraAvailableMemoryNextRound) (void)memory.shrink();
      else memory.grow(extraAvailableMemoryNextRound);
    }

    return generateGarbageCollectionSummary(previousStatistics, memory);
  }

  void mark(const GarbageCollected<>& target) noexcept {
    select<ActiveSetMemory>().mark(static_cast<const std::byte*>(target.destination), target.capacity);
  }

  void split() {
    select<ActiveSetMemory>().push();
  }

  void untie() {
    select<ActiveSetMemory>().pop();
  }
}