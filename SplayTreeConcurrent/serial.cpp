
#include "splayset.hpp"
#include "stopwatch.hpp"
#include "rc4prng.hpp"

#include <iostream>
#include <cstdint>

const size_t concurrency = 4,
             item_count = 200000,
             group_size = 100,
             group_accesses = 2000,
             group_count = 1000;

const char rand_seed[] = "test ukolu";

void test_group_serial (rc4prng<>& rng, splayset<uint64_t>&st)
{
	uint64_t begin = rng.random(), mult = rng.random();
	for (size_t i = 0; i < group_accesses; ++i)
		st.find (begin + mult * rng.random (group_size));
}

int main_serial()
{
	splayset<uint64_t> st;

	rc4prng<> thread_rng[concurrency];
	for (size_t tid = 0; tid < concurrency; ++tid)
		thread_rng[tid].load_key (rand_seed, rand_seed + 10 - tid); // :]

	bpp::Stopwatch stopwatch (true);

	for (size_t tid = 0; tid < concurrency; ++tid)
		for (size_t i = 0; i < item_count; ++i)
			st.insert (thread_rng[tid].random());

	stopwatch.stop();
	std::cout << stopwatch.getMiliseconds() << std::endl;
	stopwatch.start();

	for (size_t tid = 0; tid < concurrency; ++tid)
		for (size_t grp = 0; grp < group_count; ++grp)
			test_group_serial (thread_rng[tid], st);

	stopwatch.stop();
	std::cout << stopwatch.getMiliseconds() << std::endl;
	return 0;
}
