#pragma once

#include <cfp/meta/preprocessor/cat.h>

#ifdef __GNUC__
#ifndef NDEBUG

// implements a barrier to avoid out-of-order execution
// see https://www.intel.com/content/dam/www/public/us/en/documents/white-papers/ia-32-ia-64-benchmark-code-execution-paper.pdf
// and also chapter 17.5 in
// https://www.intel.com/content/dam/www/public/us/en/documents/manuals/64-ia-32-architectures-software-developer-vol-3b-part-2-manual.pdf
// for references about the RDTSC/RDTSCP instruction format and units
#define STOPWATCH_PRE(HIGH, LOW)                                                              \
asm volatile ("CPUID\n\t"                                                                     \
              "RDTSC\n\t"                                                                     \
              "mov %%edx, %0\n\t"                                                             \
              "mov %%eax, %1\n\t": "=r" (HIGH), "=r" (LOW):: "%rax", "%rbx", "%rcx", "%rdx");

// Note: CPUID, prevents contamination from instructions located *after* the RDTSCP block
#define STOPWATCH_POST(HIGH, LOW)                                                             \
asm volatile ("RDTSCP\n\t"                                                                    \
              "mov %%edx, %0\n\t"                                                             \
              "mov %%eax, %1\n\t"                                                             \
              "CPUID\n\t": "=r" (HIGH), "=r" (LOW):: "%rax", "%rbx", "%rcx", "%rdx");

#else
#define STOPWATCH_PRE(HIGH, LOW)
#define STOPWATCH_POST(HIGH, LOW)
#endif

#endif
