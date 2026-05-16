#include <benchmark/benchmark.h>
#include <string>

static void BM_StringCreation(benchmark::State& state) {
  for (auto _ : state)
    std::string x = "hello world, this string is a lot more longer";
}
// Register the function as a benchmark
BENCHMARK(BM_StringCreation);

// Define another benchmark
static void BM_StringCopy(benchmark::State& state) {
  std::string x = "hello world, this string is a lot more longer";
  for (auto _ : state)
    std::string copy(x);
}
BENCHMARK(BM_StringCopy);

BENCHMARK_MAIN();