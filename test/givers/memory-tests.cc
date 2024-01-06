#include <cstring>
#include <gtest/gtest.h>
#include "givers/memory.hh"

TEST(Memory, instantiation) {
  mamba::GarbageCollected<char> string = gather(56, mamba::GarbageCollectionGeneration::Eden);
  ASSERT_EQ(string.capacity, 56);
  ASSERT_EQ(string.lifetime, mamba::GarbageCollectionGeneration::Eden);
  ASSERT_NE(string.destination, nullptr);
}

TEST(Memory, consistentWritesAndReads) {
  auto read = [](const mamba::GarbageCollected<char>& string){
    volatile char value;
    for (int byte = 0; byte < 100; ++byte) value = string.destination[byte];
  };

  mamba::GarbageCollected<char> string = gather(100, mamba::GarbageCollectionGeneration::Eden);
  EXPECT_NO_FATAL_FAILURE(read(string));

  const char content[] = "Ah yes, the negotiator, general Kenobi.";
  EXPECT_NO_FATAL_FAILURE({ (void)std::memcpy(string.destination, content, sizeof(content)); });
  EXPECT_NO_FATAL_FAILURE(read(string));
}

