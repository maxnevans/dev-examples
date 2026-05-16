#include <array>
#include <benchmark/benchmark.h>
#include <memory_resource>
#include <string>
#include <vector>
#include <list>


static void BM_StringCreation(benchmark::State &state) {
  for (auto _ : state)
    std::string x = "hello world, this string is a lot more longer";
}
// Register the function as a benchmark
BENCHMARK(BM_StringCreation);

// Define another benchmark
static void BM_StringCopy(benchmark::State &state) {
  std::string x = "hello world, this string is a lot more longer";
  for (auto _ : state)
    std::string copy(x);
}
BENCHMARK(BM_StringCopy);

static void BM_VectorNew(benchmark::State &state) {
  for (auto _ : state) {
    std::vector<int> Array;
  }
}

static void BM_VectorNewItem_2(benchmark::State &state) {
  for (auto _ : state) {
    std::vector<int> Array;
    for (int i = 0; i < 2; ++i) {
      Array.push_back(10);
    }
  }
}

static void BM_VectorNewItem_16(benchmark::State &state) {
  for (auto _ : state) {
    std::vector<int> Array;
    for (int i = 0; i < 16; ++i) {
      Array.push_back(10);
    }
  }
}

static void BM_VectorNewItem_16_PREALLOC(benchmark::State &state) {
  for (auto _ : state) {
    static const int TotalElements = 16;
    std::vector<int> Array;
    Array.reserve(TotalElements);
    for (int i = 0; i < TotalElements; ++i) {
      Array.push_back(10);
    }
  }
}

static void BM_VectorNewItem_16_InlineAllocator(benchmark::State &state) {
  for (auto _ : state) {
    static const int TotalElements = 16;
    // 1. Create a raw array or buffer on the stack
    alignas(int) char stack_buffer[TotalElements * sizeof(int)];

    // 2. Wrap it inside a monotonic buffer resource
    std::pmr::monotonic_buffer_resource mem_pool(stack_buffer,
                                                 sizeof(stack_buffer));
    // 3. Define the vector using the pmr namespace and pass the pool resource
    std::pmr::vector<int> Array(&mem_pool);

    for (int i = 0; i < TotalElements; ++i) {
      Array.push_back(10);
    }
  }
}

static void
BM_VectorNewItem_16_InlineAllocator_WithReserve(benchmark::State &state) {
  for (auto _ : state) {
    static const int TotalElements = 16;
    static const int ReserveForReallocation = 1000;
    std::array<std::byte,
               (TotalElements + ReserveForReallocation) * sizeof(int)>
        buffer;

    // 2. Wrap it inside a monotonic buffer resource
    std::pmr::monotonic_buffer_resource mem_pool(buffer.data(), buffer.size());
    // 3. Define the vector using the pmr namespace and pass the pool resource
    std::pmr::vector<int> Array(&mem_pool);

    for (int i = 0; i < TotalElements; ++i) {
      Array.push_back(10);
    }
  }
}

static void
BM_VectorNewItem_16_InlineAllocator_PREALLOC(benchmark::State &state) {
  for (auto _ : state) {
    static const int TotalElements = 16;
    // 1. Create a raw array or buffer on the stack
    alignas(int) char stack_buffer[TotalElements * sizeof(int)];

    // 2. Wrap it inside a monotonic buffer resource
    std::pmr::monotonic_buffer_resource mem_pool(stack_buffer,
                                                 sizeof(stack_buffer));
    // 3. Define the vector using the pmr namespace and pass the pool resource
    std::pmr::vector<int> Array(&mem_pool);
    Array.reserve(TotalElements);

    for (int i = 0; i < TotalElements; ++i) {
      Array.push_back(10);
    }
  }
}

static void BM_ListNew(benchmark::State &state) {
  for (auto _ : state) {
    std::list<int> List;
  }
}

static void BM_ListNewItem_2(benchmark::State &state) {
  for (auto _ : state) {
    std::list<int> List;
    for (int i = 0; i < 2; ++i) {
      List.push_back(10);
    }
  }
}

static void BM_ListNewItem_16(benchmark::State &state) {
  for (auto _ : state) {
    std::list<int> List;
    for (int i = 0; i < 16; ++i) {
      List.push_back(10);
    }
  }
}


static void BM_ListNewItem_16_InlineAllocator(benchmark::State &state) {
  for (auto _ : state) {
    static const int TotalElements = 16;
    // 1. Create a raw array or buffer on the stack
    alignas(int) char stack_buffer[TotalElements * sizeof(int)];

    // 2. Wrap it inside a monotonic buffer resource
    std::pmr::monotonic_buffer_resource mem_pool(stack_buffer,
                                                 sizeof(stack_buffer));
    // 3. Define the list using the pmr namespace and pass the pool resource
    std::pmr::list<int> List(&mem_pool);

    for (int i = 0; i < TotalElements; ++i) {
      List.push_back(10);
    }
  }
}

static void
BM_ListNewItem_16_InlineAllocator_WithReserve(benchmark::State &state) {
  for (auto _ : state) {
    static const int TotalElements = 16;
    static const int ReserveForReallocation = 1000;
    std::array<std::byte,
               (TotalElements + ReserveForReallocation) * sizeof(int)>
        buffer;

    // 2. Wrap it inside a monotonic buffer resource
    std::pmr::monotonic_buffer_resource mem_pool(buffer.data(), buffer.size());
    // 3. Define the list using the pmr namespace and pass the pool resource
    std::pmr::list<int> List(&mem_pool);

    for (int i = 0; i < TotalElements; ++i) {
      List.push_back(10);
    }
  }
}

static void BM_ArrayNew(benchmark::State &state) {
  for (auto _ : state) {
    std::array<int, 0> Array;
  }
}

static void BM_ArrayNewItem_2(benchmark::State &state) {
  for (auto _ : state) {
    std::array<int, 2> Array;
    for (int i = 0; i < 2; ++i) {
      Array[i] = 10;
    }
  }
}

static void BM_ArrayNewItem_16(benchmark::State &state) {
  for (auto _ : state) {
    std::array<int, 16> Array;
    for (int i = 0; i < 16; ++i) {
      Array[i] = 10;
    }
  }
}

BENCHMARK(BM_VectorNew);
BENCHMARK(BM_VectorNewItem_2);
BENCHMARK(BM_VectorNewItem_16);
BENCHMARK(BM_VectorNewItem_16_PREALLOC);
BENCHMARK(BM_VectorNewItem_16_InlineAllocator);
BENCHMARK(BM_VectorNewItem_16_InlineAllocator_WithReserve);
BENCHMARK(BM_VectorNewItem_16_InlineAllocator_PREALLOC);

BENCHMARK(BM_ListNew);
BENCHMARK(BM_ListNewItem_2);
BENCHMARK(BM_ListNewItem_16);
BENCHMARK(BM_ListNewItem_16_InlineAllocator);
BENCHMARK(BM_ListNewItem_16_InlineAllocator_WithReserve);

BENCHMARK(BM_ArrayNew);
BENCHMARK(BM_ArrayNewItem_2);
BENCHMARK(BM_ArrayNewItem_16);

BENCHMARK_MAIN();