#include <memory>

#include <benchmark/benchmark.h>
#include "core/givers/ActiveSetMemory.hh"

const size_t randomSizes[] = { 2348, 5337, 5148, 4682, 7774, 7141, 2563, 915, 4137, 8396, 7404, 9117, 4665, 9096, 5194,
                            813, 3499, 9308, 5930, 9851, 9050, 263, 8776, 3797, 1936, 922, 16, 3510, 6456, 4159, 7146,
                            4202, 8649, 7339, 7093, 1089, 9708, 1421, 7694, 1884, 4085, 1377, 2159, 5037, 3614, 1179,
                            4118, 4529, 3810, 3001, 5294, 9432, 4928, 2537, 2848, 6920, 4047, 60, 6302, 8269, 91, 1447,
                            5296, 7060, 641, 5027, 1776, 6737, 5102, 2732, 8619, 9551, 4111, 7677, 6904, 461, 3889,
                            1635, 6914, 5004, 9175, 5523, 6865, 6851, 3572, 8444, 3503, 3743, 4071, 7663, 9513, 4720,
                            4213, 7142, 5113, 2439, 1584, 1537, 4064, 4753 };

const size_t smallSizes[] = { 35, 23, 71, 64, 16, 39, 32, 74, 29, 52, 84, 71, 89, 76, 74, 21, 97, 99, 38, 57, 44, 43,
                              4, 87, 75, 48, 52, 24, 66, 100, 77, 58, 51, 79, 87, 30, 67, 40, 99, 59, 24, 31, 94, 16,
                              85, 17, 44, 63, 18, 36, 12, 62, 100, 24, 59, 28, 46, 78, 90, 98, 80, 35, 52, 92, 15, 77,
                              25, 62, 41, 65, 49, 42, 52, 1, 77, 18, 93, 92, 8, 32, 38, 56, 56, 11, 65, 67, 73, 25, 28,
                              28, 87, 92, 34, 28, 40, 50, 29, 61, 56, 13 };

const size_t mediumSizes[] = { 1453, 1453, 957, 3595, 3757, 1823, 1061, 1485, 3133, 2195, 1251, 3053, 2493, 3890, 2834,
                               2483, 2057, 3397, 2430, 1775, 3564, 1597, 2440, 355, 3749, 2661, 3038, 1954, 1057, 3403,
                               3087, 2108, 3142, 833, 3576, 1150, 2500, 1178, 527, 2370, 969, 1789, 3176, 453, 2638,
                               143, 839, 149, 901, 2725, 312, 112, 697, 1190, 3577, 126, 1847, 2612, 2819, 193, 1343,
                               3586, 1365, 2402, 2958, 3466, 2330, 2546, 1634, 1472, 730, 1631, 3658, 706, 889, 626,
                               2014, 3774, 3361, 3360, 728, 2628, 2803, 1656, 2335, 1689, 3316, 508, 1332, 2073, 1441,
                               525, 3011, 2222, 3874, 1623, 524, 1256, 3744, 3784 };

const size_t largeSizes[] = { 7360, 6987, 8797, 8830, 6802, 8941, 7164, 8814, 9928, 9860, 4162, 8155, 5464, 8743, 9340,
                           8100, 5681, 6377, 7497, 9349, 8193, 9540, 4258, 4118, 6020, 7913, 7155, 7433, 9372, 7380,
                           8280, 6729, 7505, 6300, 7263, 9231, 4221, 5049, 7247, 7098, 9233, 8351, 4668, 9336, 6556,
                           6281, 5886, 9449, 4844, 9645, 5288, 6485, 4021, 7524, 7036, 4149, 7186, 6900, 7528, 4731,
                           4521, 9472, 4808, 6056, 6586, 6809, 5128, 4681, 7561, 6420, 5528, 4406, 6586, 4597, 6524,
                           5110, 4789, 4524, 7678, 5804, 9583, 9054, 8150, 8390, 7700, 8087, 4686, 9660, 6352, 9881,
                           7275, 4021, 7433, 5151, 9570, 6708, 4371, 7132, 4368, 9055 };

static void incrementallyRisingSizeAllocationsWithBookmarks(benchmark::State& state) {
  for (auto _ : state) {
    auto& memory = mamba::book();
    memory.setGrowthFactor(60);
    for (int size = 0; size < 1000; ++size)
      mamba::Bookmark<int> data = memory.gather(size);
  }
}

static void incrementallyRisingSizeAllocationsWithNew(benchmark::State& state) {
  for (auto _ : state) {
    for (int size = 0; size < 2 * 1000; ++size)
      std::unique_ptr<int[]> data = std::make_unique<int[]>(size);
  }
}

static void randomSizeAllocationBookmark(benchmark::State& state) {
  for (auto _ : state) {
    auto& memory = mamba::book();
    mamba::Bookmark<char> strings[100];
    for (int size = 0; size < 100; ++size)
      strings[size] = memory.gather(randomSizes[size]);
  }
}

static void randomSizeAllocationNew(benchmark::State& state) {
  for (auto _ : state) {
    std::unique_ptr<char[]> strings[100];
    for (int size = 0; size < 100; ++size)
      strings[size] = std::make_unique<char[]>(randomSizes[size]);
  }
}

static void smallSizeAllocationsBookmark(benchmark::State& state) {
  for (auto _ : state) {
    auto& memory = mamba::book();
    mamba::Bookmark<char> strings[100];
    for (int size = 0; size < 100; ++size)
      strings[size] = memory.gather(smallSizes[size]);
  }
}

static void smallSizesAllocationsNew(benchmark::State& state) {
  for (auto _ : state) {
    std::unique_ptr<char[]> strings[100];
    for (int size = 0; size < 100; ++size)
      strings[size] = std::make_unique<char[]>(smallSizes[size]);
  }
}

static void mediumSizesAllocationBookmark(benchmark::State& state) {
  for (auto _ : state) {
    auto& memory = mamba::book();
    mamba::Bookmark<char> strings[100];
    for (int size = 0; size < 100; ++size)
      strings[size] = memory.gather(mediumSizes[size]);
  }
}

static void mediumSizesAllocationNew(benchmark::State& state) {
  for (auto _ : state) {
    std::unique_ptr<char[]> strings[100];
    for (int size = 0; size < 100; ++size)
      strings[size] = std::make_unique<char[]>(mediumSizes[size]);
  }
}

static void largeSizeAllocationsBookmark(benchmark::State& state) {
  for (auto _ : state) {
    auto& memory = mamba::book();
    mamba::Bookmark<char> strings[100];
    for (int size = 0; size < 100; ++size)
      strings[size] = memory.gather(largeSizes[size]);
  }
}

static void largeSizeAllocationsNew(benchmark::State& state) {
  for (auto _ : state) {
    std::unique_ptr<char[]> strings[100];
    for (int size = 0; size < 100; ++size)
      strings[size] = std::make_unique<char[]>(largeSizes[size]);
  }
}

BENCHMARK(incrementallyRisingSizeAllocationsWithBookmarks);
BENCHMARK(incrementallyRisingSizeAllocationsWithNew);
BENCHMARK(randomSizeAllocationBookmark);
BENCHMARK(randomSizeAllocationNew);
BENCHMARK(smallSizeAllocationsBookmark);
BENCHMARK(smallSizesAllocationsNew);
BENCHMARK(mediumSizesAllocationBookmark);
BENCHMARK(mediumSizesAllocationNew);
BENCHMARK(largeSizeAllocationsBookmark);
BENCHMARK(largeSizeAllocationsNew);

BENCHMARK_MAIN();
/*==========================================================================================
Run on (4 X 2700 MHz CPU s)
CPU Caches:
  L1 Data 24 KiB (x4)
  L1 Instruction 32 KiB (x4)
  L2 Unified 4096 KiB (x1)
Load Average: 4.67, 3.09, 2.74
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------------------------
Benchmark                                                Time             CPU   Iterations
------------------------------------------------------------------------------------------
incrementallyRisingSizeAllocationsWithBookmarks      52782 ns        52483 ns        13083
incrementallyRisingSizeAllocationsWithNew          2652595 ns      2636182 ns          268
randomSizeAllocationBookmark                          7423 ns         7378 ns        92525
randomSizeAllocationNew                             793886 ns       789277 ns          889
smallSizeAllocationsBookmark                          7370 ns         7327 ns        95963
smallSizesAllocationsNew                             33816 ns        33626 ns        21267
mediumSizesAllocationBookmark                         7259 ns         7218 ns        96664
mediumSizesAllocationNew                            351261 ns       349152 ns         1971
largeSizeAllocationsBookmark                          7183 ns         7144 ns        97023
largeSizeAllocationsNew                            1145912 ns      1138546 ns          556
============================================================================================*/