#pragma once

#include <cstddef>
#include <cstdint>
#include <thread>

#if defined __GNUC__
#include <tbb/concurrent_unordered_set.h>
#include <tbb/concurrent_vector.h>
#define concurrency tbb
#define Concurrency tbb
#else
#include <concurrent_unordered_set.h>
#include <concurrent_vector.h>
#endif

#ifdef _DEBUG
const size_t concurrency_ = 4,
item_count = 600,
group_size = 10,
group_accesses = 20,
group_count = 10;
#else
const size_t concurrency_ = 4,
item_count = 200000, 
group_size = 100,
group_accesses = 2000,
group_count = 1000;
#endif // _DEBUG

const char rand_seed[] = "test ukolu";
