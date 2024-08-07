//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "benchmark/benchmark.h"

#include "VariantBenchmarks.h"

using namespace VariantBenchmarks;

BENCHMARK_TEMPLATE(BM_Visit, 1, 1);
BENCHMARK_TEMPLATE(BM_Visit, 1, 2);
BENCHMARK_TEMPLATE(BM_Visit, 1, 3);
BENCHMARK_TEMPLATE(BM_Visit, 1, 4);
BENCHMARK_TEMPLATE(BM_Visit, 1, 5);
BENCHMARK_TEMPLATE(BM_Visit, 1, 6);
BENCHMARK_TEMPLATE(BM_Visit, 1, 7);
BENCHMARK_TEMPLATE(BM_Visit, 1, 8);
BENCHMARK_TEMPLATE(BM_Visit, 1, 9);
BENCHMARK_TEMPLATE(BM_Visit, 1, 10);
BENCHMARK_TEMPLATE(BM_Visit, 1, 20);
BENCHMARK_TEMPLATE(BM_Visit, 1, 30);
BENCHMARK_TEMPLATE(BM_Visit, 1, 40);
BENCHMARK_TEMPLATE(BM_Visit, 1, 50);
BENCHMARK_TEMPLATE(BM_Visit, 1, 60);
BENCHMARK_TEMPLATE(BM_Visit, 1, 70);
BENCHMARK_TEMPLATE(BM_Visit, 1, 80);
BENCHMARK_TEMPLATE(BM_Visit, 1, 90);
BENCHMARK_TEMPLATE(BM_Visit, 1, 100);

BENCHMARK_MAIN();
