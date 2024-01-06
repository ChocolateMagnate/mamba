#include <cmath>
#include "GarbageBitPage.hh"
namespace mamba {
  GarbageBitsetPage::GarbageBitsetPage(std::byte* origin) noexcept : beginning{origin} { }

  void GarbageBitsetPage::markAsGarbage(const std::byte* slab, const int amount) noexcept {
    const ptrdiff_t bytesBetweenBeginningAndRequestedSlab = slab - beginning;
    const size_t offset = std::ceil(bytesBetweenBeginningAndRequestedSlab / SlabSize);
    markAsGarbage(offset, amount);
  }

  void GarbageBitsetPage::markAsGarbage(const unsigned int offset, const int amount) noexcept {
    if (offset <= PageSize) for (int next = 0; next < amount; ++next) bitmask.reset(offset + next);
  }

  unsigned int GarbageBitsetPage::getGarbageSize() const noexcept {
    return (bitmask.size() - bitmask.count()) * SlabSize;
  }

  unsigned int GarbageBitsetPage::getGarbageSizeBefore(const void* const separator) const noexcept {
    const long targetSlabIndex = (static_cast<const std::byte*>(separator) - beginning) / SlabSize;
    if (targetSlabIndex > SlabsInPage) return getGarbageSize();
    unsigned int afterSeparatorGarbageSize = 0;
    for (unsigned int afterIndex = targetSlabIndex; afterIndex < PageSize; ++afterIndex)
      if (!bitmask.test(afterIndex)) afterSeparatorGarbageSize += SlabSize;
    return getGarbageSize() - afterSeparatorGarbageSize;
  }

  GarbageCollected<> GarbageBitsetPage::yeildNextGarbageSector(const bool shouldMoveSlabCounter) noexcept {
    const std::bitset<SlabSize> forwardSlabs = bitmask >> (yieldingGarbageSlabIndex + 1);
    if (forwardSlabs.none()) return {0, nullptr};

    // This solution is rather generic and linear, and below we try to find the first set bit and
    // the size of the contigious set bits. Up to this date, there is no constant time solution
    // that could use the magic of bitwise operations to solve it faster, so whenever any more
    // efficient idea comes down, feel free to modify this section.
    size_t index = yieldingGarbageSlabIndex, difference = 0;
    while (!bitmask.test(index)) ++index;
    while (bitmask.test(index + difference)) ++difference;
    if (shouldMoveSlabCounter) yieldingGarbageSlabIndex = index;
    return {difference, beginning + index * SlabSize};
  }

  GarbageCollected<> GarbageBitsetPage::yieldNextAliveSector(const bool shouldMoveSlabCounter) noexcept {
    const std::bitset<SlabSize> forwardSlabs = bitmask >> (yieldingAliveSlabIndex + 1);
    if (forwardSlabs.none()) return {0, nullptr};

    size_t index = yieldingAliveSlabIndex, difference = 0;
    while (bitmask.test(index)) ++index;
    while (!bitmask.test(index + difference)) ++difference;
    if (shouldMoveSlabCounter) yieldingAliveSlabIndex = index;
    return {difference, beginning + index * SlabSize};
  }
}  // namespace mamba
