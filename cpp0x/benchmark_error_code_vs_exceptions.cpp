#include "benchmark/benchmark.h"
#include <system_error>
#include <iostream>

// #include "config/errors.hpp"

std::error_code __attribute__ ((noinline)) do_stuff(int remaining_iterations, std::function<std::error_code()> const& error_func) {
  if (remaining_iterations > 0) {
    return do_stuff(remaining_iterations - 1, error_func);
  } else {
    return error_func();
  }
}

static void BM_ReturnNoErrorCode(benchmark::State& state) {
  std::function<std::error_code()> func = []() -> std::error_code {
    return {};
  };
  for (auto _ : state) {
    std::error_code ec = do_stuff(state.range(0), func);
    benchmark::DoNotOptimize(ec.message().size());
  }
}
BENCHMARK(BM_ReturnNoErrorCode)->Arg(10)->Arg(50)->Arg(100)->Arg(500);

static void BM_ReturnErrorCode(benchmark::State& state) {
  std::function<std::error_code()> func = []() -> std::error_code {
        return std::error_code(33, std::generic_category());
  };
  for (auto _ : state) {
    std::error_code ec = do_stuff(state.range(0), func);
    benchmark::DoNotOptimize(ec.message().size());
  }
}
BENCHMARK(BM_ReturnErrorCode)->Arg(10)->Arg(50)->Arg(100)->Arg(500);
const char * err_msg = "an error";
struct MyException{};
static void BM_ThrowException(benchmark::State& state) {
  std::function<std::error_code()> func = []() -> std::error_code {
        // throw std::runtime_error{err_msg};
        throw MyException{};
  };
  for (auto _ : state) {
    try {
      do_stuff(state.range(0), func);
    } catch (MyException& error) {
    }
  }
}
BENCHMARK(BM_ThrowException)->Arg(10)->Arg(50)->Arg(100)->Arg(500);

BENCHMARK_MAIN();
