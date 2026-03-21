#include "benchmark/benchmark.h"
#include "matrix.hpp"

bool Matrix::strassen = false;

static void BM_mult(benchmark::State &state) {
  for (auto _ : state) {
    Matrix::strassen = state.range(0);
    size_t aa = state.range(1);

    Matrix a(aa);
    Matrix b(aa);
    double c = 0;

    for (size_t y = 0; y < aa; ++y) {
      for (size_t x = 0; x < aa; ++x) {
        a[y][x] = c++;
      }
    }

    for (size_t y = 0; y < aa; ++y) {
      for (size_t x = 0; x < aa; ++x) {
        b[y][x] = c++;
      }
    }

    auto dudum = a * b;
    (void)dudum;
  }
}

BENCHMARK(BM_mult)->RangeMultiplier(2)->Ranges({{0, 1}, {64, 4096}});

BENCHMARK_MAIN();
