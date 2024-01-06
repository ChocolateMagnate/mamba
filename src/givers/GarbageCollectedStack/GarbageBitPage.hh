/*+================================================================================================
  File:        GarbageBitPage.hh

  Summary:     Describes kernel memory page-sized units internally used for alinging memory into
               fixed-size blocks. The pages use the slab allocator where a single slab equals 64
               bytes (common cacheline size), and each page in the sense of Mamba represents a
               page of virtual memory as a 64-byte bitmask where each bit represents a cahceline-
               sized slab, followed by the pointer to said memory region. The primary use of pages
               in Mamba lies in the ability to treck garbage data in ActiveSetMemory pool and assist
               the major garbage collector in what memory region should be reclaimed.

  Constants:   SlabSize, PageSize

  Classes:     GarbageSector, GarbageBitPage

  Functions:   None

  Available under Apache Licence v2. Mamba Authors (2023)
=================================================================================================+*/
#pragma once

#include <bitset>

#include "givers/memory.hh"

namespace mamba {
  constexpr auto SlabsInPage = 64;
  constexpr auto SlabSize = 64 * 8;
  constexpr auto PageSize = 64 * 64;

  /// Conveys meta-data about a single kernel page allocated within GarbgageCollectedStack.
  /// The use of pages arise from the need to explicitly keep treck garbage slabs to reclaim
  /// them during major GC.
  class GarbageBitsetPage {
    public:
      GarbageBitsetPage() = default;
      GarbageBitsetPage(const GarbageBitsetPage&) = default;
      GarbageBitsetPage(GarbageBitsetPage&&) = default;

      /// Initialises the page from the base pointer where its memory begins.
      /// @param origin The pointer to the beginning of the memory page.
      explicit GarbageBitsetPage(std::byte* origin) noexcept;

      GarbageBitsetPage& operator=(const GarbageBitsetPage&) = default;
      GarbageBitsetPage& operator=(GarbageBitsetPage&&) = default;

      /// Marks the slab at the specified location as garbage.
      /// @param slab The pointer to the garbage cacheline.
      /// @param amount (Optional) The number of slabs that make up the garbage value.
      void markAsGarbage(const std::byte* slab, int amount = 1) noexcept;

      /// Sets the slab at the specified position as garbage.
      /// @param offset The index of the slab in bitmask that must be marked as garbage.
      /// @param amount (Optional) The number of slabs that make up the garbage sector.
      void markAsGarbage(unsigned int offset, int amount = 1) noexcept;

      /// Computes the size of the garbage slabs associated with this page.
      /// @return The size of all garbage sectors that begin in this page.
      [[nodiscard]] unsigned int getGarbageSize() const noexcept;

      /// Computes the size of the garbage slabs in this page before the point in memory
      /// @param separator The pointer to the end where the difference is computed.
      /// @return  The size of all garbage sectors before the specified memory location.
      [[nodiscard]] unsigned int getGarbageSizeBefore(const void* separator) const noexcept;

      /// Provides the reference to the next garbage sector in the page. The call to
      /// this function will progress the index of the trackable items, therefore the
      /// return value must be caught and utilised, unless the off flag is set.
      /// @param shouldMoveSlabCounter (Optional) Tells if the slab position must be
      /// updated after the call. If set to true, the call to this method will return
      /// the same garbage sector as it did the first time, and this can be used to
      /// be able to catch the reference if it's needed in the future. Other than that,
      /// it is set to false by default, which will move the slab index after the call.
      /// @return A garbage sector object containing the pointer and size of the garbage data.
      GarbageCollected<> yeildNextGarbageSector(bool shouldMoveSlabCounter = false) noexcept;

      /// Provides the reference to the next alive sector in the page. The call to this
      /// function will progress the index of the treckable items, therefore the return
      /// value must be caught and utilised, unless the off flag is set.
      /// @param shouldMoveSlabCounter (Optional) Tells if the slab position must be
      /// updated after the call. If set to true, the call to this method will return
      /// the same alive sector as it did the first time, and this can be used to
      /// be able to catch the reference if it's needed in the future. Other than that,
      /// it is set to false by default, which will move the slab index after the call.
      /// @return An alive sector object containing the pointer and size of the alive data.
      GarbageCollected<> yieldNextAliveSector(bool shouldMoveSlabCounter = false) noexcept;

    private:
      std::bitset<SlabSize> bitmask;
      std::byte* beginning{};
      unsigned int yieldingGarbageSlabIndex = 0, yieldingAliveSlabIndex = 0;
  };
}