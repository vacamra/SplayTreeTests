#include "Settings.h"

#include "splayset_concurrent.hpp"
#include "stopwatch.hpp"
#include "rc4prng.hpp"
#include "testingUtils.h"

#include <unordered_set>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <cstdint>

void test_group_single_thread(rc4prng<>& rng, const std::unordered_set<uint64_t>& us, splayset<uint64_t>&st)
{
	uint64_t begin = rng.random(), mult = rng.random();
	for (size_t i = 0; i < group_accesses; ++i) {
		int64_t query = begin + mult * rng.random(group_size);
		auto result = st.find(query);
		auto result2 = us.find(query);
		my_assert((result == nullptr) == (result2 == us.end()));
		my_assert(!result || (*result2 == result->data));
	}
}

// Otestuje to insert a find single_thread
int main_singleThread()
{
	splayset<uint64_t> st;
	std::unordered_set<uint64_t> us;

	rc4prng<> thread_rng[concurrency_];
	for (size_t tid = 0; tid < concurrency_; ++tid)
		thread_rng[tid].load_key(rand_seed, rand_seed + 10 - tid); // :]

	std::vector<std::string> result;

	bpp::Stopwatch stopwatch(true);

	for (size_t tid = 0; tid < concurrency_; ++tid)
		for (size_t i = 0; i < item_count; ++i) {
			auto value = thread_rng[tid].random();
			st.insert(value);
			us.insert(value);
		}

	stopwatch.stop();
	std::cout << stopwatch.getMiliseconds() << std::endl;
	std::cout << "Tree size:" << st.size() << std::endl;

	stopwatch.start();

	for (size_t tid = 0; tid < concurrency_; ++tid)
		for (size_t grp = 0; grp < group_count; ++grp)
			test_group_single_thread(thread_rng[tid], us, st);

	stopwatch.stop();
	std::cout << stopwatch.getMiliseconds() << std::endl;
	std::cout << "Tree size:" << st.size() << std::endl;

	testUSnSt(us, st);
	
	return 0;
}
