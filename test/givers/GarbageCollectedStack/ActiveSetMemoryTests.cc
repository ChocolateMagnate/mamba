#include <gtest/gtest.h>
#include "givers/GarbageCollectedStack/ActiveSetMemory.hh"

TEST(ActiveSetMemory, instantiation) {
  EXPECT_NO_FATAL_FAILURE({ mamba::ActiveSetMemory memory; });
}

TEST(ActiveSetMemory, gatheringExactMemoryBoundary) {
  mamba::ActiveSetMemory memory;
  const std::byte* first = memory.gather(400);
  const std::byte* second = memory.gather(500);
  const std::byte* third = memory.gather(4);
  const ptrdiff_t firstDifference = second - first;
  const ptrdiff_t secondDifference = third - second;
  EXPECT_EQ(firstDifference, 400);
  EXPECT_EQ(secondDifference, 500);
}

TEST(ActiveSetMemory, pushingAndPopping) {
  mamba::ActiveSetMemory memory;
  memory.push();
  const std::byte* bottom = memory.gather(40);
  std::byte* top = memory.top();
  EXPECT_EQ(top, bottom + 40);
  memory.pop();
  top = memory.top();
  EXPECT_EQ(top, bottom);
}

TEST(ActiveSetMemory, declineGatheringIfNotEnoughCapacity) {
  mamba::ActiveSetMemory memory;
  const std::byte* result = memory.gather(4 * mamba::DefaultStackSize);
  ASSERT_EQ(result, nullptr);
}

TEST(ActiveSetMemory, factorsAreValidated) {
  mamba::ActiveSetMemory memory;
  ASSERT_EQ(memory.getPreservationFactor(), 0);
  memory.setPreservationFactor(105);
  ASSERT_EQ(memory.getPreservationFactor(), 0);
  memory.setPreservationFactor(50);
  ASSERT_EQ(memory.getPreservationFactor(), 50);

  ASSERT_EQ(memory.getGrowthFactor(), 2);
  memory.setGrowthFactor(0);
  ASSERT_EQ(memory.getGrowthFactor(), 2);
  memory.setGrowthFactor(3);
  ASSERT_EQ(memory.getGrowthFactor(), 3);
}

TEST(ActiveSetMemory, shrinkFailsWithoutMarking) {
  mamba::ActiveSetMemory memory;
  // We purposefully increase the pool size by 5 so that its current size would be
  // multiplier of 6 so that we could shrink it by half and get 3.
  memory.grow(mamba::DefaultStackSize * 5);
  const bool isShrunk = memory.shrink();
  ASSERT_FALSE(isShrunk); //Shrink must fail if no memory was marked as garbage.
  ASSERT_EQ(memory.getMemoryUsage().allocatedMemorySize, mamba::DefaultStackSize * 6);
}

TEST(ActiveSetMemory, shrinkSuccedsWithSufficientlyGarbageMarks) {
  mamba::ActiveSetMemory memory;
  memory.grow(mamba::DefaultStackSize * 5);
  ASSERT_EQ(memory.getMemoryUsage().allocatedMemorySize, mamba::DefaultStackSize * 6);
  const std::byte* garbage = memory.gather(mamba::PageSize * 4);
  memory.mark(garbage, mamba::PageSize * 4);
  const bool isShrunk = memory.shrink();
  ASSERT_TRUE(isShrunk);
  const size_t expectedShrunkSize = mamba::DefaultStackSize * 6 - mamba::PageSize * 4;
  ASSERT_EQ(memory.getMemoryUsage().allocatedMemorySize, expectedShrunkSize);
}

TEST(ActiveSetMemory, initialStatistics) {
  const mamba::ActiveSetMemory memory;
  const mamba::MemoryUsageStatistics& defaultStatistics = memory.getMemoryUsage();
  const mamba::MemoryUsageStatistics expectedStatistics = {0, 0,
     mamba::DefaultStackSize, 0, 0, 0};
  EXPECT_EQ(defaultStatistics, expectedStatistics);
}

TEST(ActiveSetMemory, correctUsedMemoryStatisticsChanges) {
  mamba::ActiveSetMemory memory;
  memory.push();
  auto* _ = memory.gather(56);
  EXPECT_EQ(memory.getMemoryUsage().usedMemorySize, 56);
  _ = memory.gather(43);
  EXPECT_EQ(memory.getMemoryUsage().usedMemorySize, 56 + 43);
  memory.pop();
  EXPECT_EQ(memory.getMemoryUsage().usedMemorySize, 0);
}

TEST(ActiveSetMemory, correctAllocatedMemoryStatisticsChanges) {
  mamba::ActiveSetMemory memory;
  ASSERT_EQ(memory.getMemoryUsage().allocatedMemorySize, mamba::DefaultStackSize);
  memory.grow(4 * mamba::DefaultStackSize);
  ASSERT_EQ(memory.getMemoryUsage().growths, 1);
  ASSERT_EQ(memory.getMemoryUsage().allocatedMemorySize, mamba::DefaultStackSize * 5);
  const std::byte* garbage = memory.gather(mamba::PageSize * 3);
  memory.mark(garbage, mamba::PageSize * 3);
  const bool isShrunk = memory.shrink();
  ASSERT_TRUE(isShrunk);
  ASSERT_EQ(memory.getMemoryUsage().shrinks, 1);
  const size_t expectedShrunkSize = mamba::DefaultStackSize * 5 - mamba::PageSize * 3;
  ASSERT_EQ(memory.getMemoryUsage().allocatedMemorySize, expectedShrunkSize);
}