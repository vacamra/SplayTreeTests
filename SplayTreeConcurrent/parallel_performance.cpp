#include "Settings.h"

#include <iostream>
#include <thread>

#include "splayset_concurrent.hpp"
#include "stopwatch.hpp"
#include "rc4prng.hpp"

void test_group_parallel_orig(rc4prng<>& rng, splayset<uint64_t>&st)
{
	uint64_t begin = rng.random(), mult = rng.random();
	for (size_t i = 0; i < group_accesses; ++i) {
		int64_t query = begin + mult * rng.random(group_size);		
		auto result = st.find(query);
	}
}

int main_parallel_performance()
{
	splayset<uint64_t> st;

	rc4prng<> thread_rng[concurrency_];
	for (size_t tid = 0; tid < concurrency_; ++tid)
		thread_rng[tid].load_key(rand_seed, rand_seed + 10 - tid); // :]

	std::thread thread[concurrency_];
	bpp::Stopwatch stopwatch(true);

	for (size_t tid = 0; tid < concurrency_; ++tid)
		thread[tid] = std::thread([tid, &thread_rng, &st]()
	{
		for (size_t i = 0; i < item_count; ++i) {
			int64_t val = thread_rng[tid].random();
			st.insert(val);
		}
	});

	for (size_t tid = 0; tid < concurrency_; ++tid)
		thread[tid].join();

	stopwatch.stop();
	std::cout << stopwatch.getMiliseconds() << std::endl;
	std::cout << "Done" << std::endl;
	stopwatch.start();

	for (size_t tid = 0; tid < concurrency_; ++tid)
		thread[tid] = std::thread([tid, &thread_rng, &st]()
	{
		for (size_t grp = 0; grp < group_count; ++grp)
			test_group_parallel_orig(thread_rng[tid], st);
	});

	for (size_t tid = 0; tid < concurrency_; ++tid)
		thread[tid].join();

	stopwatch.stop();
	std::cout << stopwatch.getMiliseconds() << std::endl;

	return 0;
}