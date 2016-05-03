#pragma once

#ifdef _DEBUG
const size_t concurrency_ = 4,
item_count = 200,
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
