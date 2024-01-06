/*+================================================================================================
  File:        memory.hh

  Summary:     Unified interface for memory-related functionalities intended to be used elsewhere
               in Mamba. Here you can find functions regarding allocations, cloning, deleting, and
               otherwise manipulating memory.

  Classes:     GarbageCollectionLevel, AllocationScope, GarbageCollectionSummary, GarbageCollected<T = void>

  Functions:   gather(size_t, AllocationScope), clone(GarbageCollected), clone(GarbageCollected&),
               clone(GarbageCollected&, AllocationScope), collect(GarbageCollectionLevel, size_t),
               split(), untie()

  Available under Apache Licence v2. Mamba Authors (2023)
=================================================================================================+*/
#pragma once

#include <cstddef>

namespace mamba {
  /// Represents the level of garbage collection. Mamba exposes 5 levels as a metric
  /// to convey the "strength" of the GC to trigger, where smaller level is weaker,
  /// faster and could be running more often, while higher levels are expected to be
  /// more aggressive and time-consuming. Note that not all garbage collection implementations
  /// may actually employ 5 levels, however they can map them to their own internal
  /// conventions as long as they obey the expectations.
  enum class GarbageCollectionGeneration {
    Eden, Young, Elder, Antique, Pernament
  };

  /// Contains the information about the GarbageCollectedStack statistics changes
  /// performed by the garbage collection call.
  struct GarbageCollectionSummary {
    size_t reclaimedMemoryInBytes, aliveMemoryInBytes;
  };

  /// Wraps the meta-data about the GarbageCollectedStack usage of arbitrary objects.
  /// This class does not automatically make data garbage-collected once you wrap a
  /// value around it, but it signifies that it must be reclaimed later.
  template<typename T = void> struct GarbageCollected {
    size_t capacity = 0;
    T* destination = nullptr;
    GarbageCollectionGeneration lifetime = GarbageCollectionGeneration::Eden;

    GarbageCollected() = default;

    template<typename K> GarbageCollected(const GarbageCollected<K>& other) {
      capacity = other.capacity;
      destination = reinterpret_cast<T*>(other.destination);
      lifetime = other.lifetime;
    }

    template<typename K> GarbageCollected& operator=(const GarbageCollected<K>& other) {
      capacity = other.capacity;
      destination = static_cast<T*>(other.destination);
      lifetime = other.lifetime;
      return *this;
    }
  };

  /// Dynamically allocates GarbageCollectedStack on garbage-collected segments.
  /// @param size The size of the data to allocate.
  /// @param lifetime The level at which the obejct must be garbage-collected.
  /// @note If the allocation request cannot be completed because the GarbageCollectedStack pool has exhausted
  /// its capacity, the returned value .destination property is set to nullptr to indicate that
  /// the JIT compiler should finalise all alive objects and perform a major garbage collection
  /// phase. This is done because Mamba GarbageCollectedStack model uses raw pointers to reference the memory
  /// location, and therefore the pool cannot grow to not invalidate all pointers to alive data.
  /// @return A GarbageCollectedStack reference object containing the information about the allocated object.
  GarbageCollected<> gather(size_t size, GarbageCollectionGeneration lifetime);

  /// Copies an object in GarbageCollectedStack.
  /// @param original The original value that must be copied.
  /// @note If the allocation request cannot be completed because the GarbageCollectedStack pool has exhausted
  /// its capacity, the returned value .destination property is set to nullptr to indicate that
  /// the JIT compiler should finalise all alive objects and perform a major garbage collection
  /// phase. This is done because Mamba GarbageCollectedStack model uses raw pointers to reference the memory
  /// location, and therefore the pool cannot grow to not invalidate all pointers to alive data.
  /// @return A GarbageCollectedStack reference object containing the information about the allocated object.
  GarbageCollected<> clone(const GarbageCollected<>& original);

  /// Clones an existing object and sets a different allocate scope level.
  /// @param original The original value that must be copied.
  /// @param lifetime The lifetime of the new object.
  /// @note If the allocation request cannot be completed because the GarbageCollectedStack pool has exhausted
  /// its capacity, the returned value .destination property is set to nullptr to indicate that
  /// the JIT compiler should finalise all alive objects and perform a major garbage collection
  /// phase. This is done because Mamba GarbageCollectedStack model uses raw pointers to reference the memory
  /// location, and therefore the pool cannot grow to not invalidate all pointers to alive data.
  /// @return A GarbageCollectedStack reference object containing the information about the allocated object.
  GarbageCollected<> clone(const GarbageCollected<>& original, GarbageCollectionGeneration lifetime);

  /// Performs garbage collection and reclaims the memory that may not be alive by the point of the call.
  /// @param level The level of the garbage collection as stated by the mamba::GarbageCollectionLevel enum
  /// class. This value specifies the strength of the request, where higher values show a stronger suggestion
  /// to do the garbage collection, providing a 5-level trade-off balance between the time it takes to reclaim
  /// GarbageCollectedStack and the speed how long the process will take.
  /// @param extraAvailableMemoryNextRound (Optional) Specifies how much GarbageCollectedStack in bytes must be
  /// available after the call to the garbage collection to proceed to the next phase. If the memory pool won't
  /// have enough memory available after the call, it will resize itself to complete the request.
  /// @return A summary of the garbage collection. Can be inspected tp gather telemetry information.
  GarbageCollectionSummary collect(GarbageCollectionGeneration level, size_t extraAvailableMemoryNextRound = 0);

  /// Marks the specified memory region as unused that will make it available to be collected by a major GC phase.
  /// @param target The garbagage-collected object that must be marked for deletion.
  void mark(GarbageCollected<>& target) noexcept;

  /// Provides the hint that the current memory context should be split, meaning that the next allocations may
  /// be performed separately from the previous context. This method provides the caller with more granular and
  /// controlled manner to regulate the composition of the memory pool and leverage additional optimisations
  /// based on divisions into multiple memory frames.
  void split();

  /// Provides the hint that the current memory context should be erased. This function can be used to tell
  /// the memory pool to destroy the current memory context and return to the previous one because the caller
  /// deems the upcoming allocations to be more efficient when mixed with the previous context.
  void untie();
}