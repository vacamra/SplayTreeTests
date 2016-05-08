#include "Settings.h"

#include "splayset_serial.hpp"
#include "stopwatch.hpp"
#include "rc4prng.hpp"
#include "testingUtils.h"

#include <unordered_set>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <cstdint>

void test_group_serial_performance(rc4prng<>& rng, splayset_serial<uint64_t>&st)
{
	uint64_t begin = rng.random(), mult = rng.random();
	for (size_t i = 0; i < group_accesses; ++i) {
		int64_t query = begin + mult * rng.random(group_size);
		auto result = st.find(query);
	}
}

int main_serial_orig_perfomance()
{
	splayset_serial<uint64_t> st;

	rc4prng<> thread_rng[concurrency_];
	for (size_t tid = 0; tid < concurrency_; ++tid)
		thread_rng[tid].load_key (rand_seed, rand_seed + 10 - tid); // :]

	std::vector<std::string> result;

	bpp::Stopwatch stopwatch (true);

	for (size_t tid = 0; tid < concurrency_; ++tid)
		for (size_t i = 0; i < item_count; ++i) {
			auto value = thread_rng[tid].random();
			st.insert(value);
		}

	stopwatch.stop();
	std::cout << stopwatch.getMiliseconds() << std::endl;
	std::cout << "Tree size:" << st.size() << std::endl;

	stopwatch.start();

	for (size_t tid = 0; tid < concurrency_; ++tid)
		for (size_t grp = 0; grp < group_count; ++grp)
			test_group_serial_performance(thread_rng[tid], st);

	stopwatch.stop();
	std::cout << stopwatch.getMiliseconds() << std::endl;
	std::cout << "Tree size:" << st.size() << std::endl;

	return 0;
}
