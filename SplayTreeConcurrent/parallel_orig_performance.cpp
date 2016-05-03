
#include <cassert>
#include <concurrent_unordered_set.h>
#include <mutex>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <cstdint>
#include <thread>

//Don't try this at home - for root access
#define private public 
#include "splayset_concurrent.hpp"
#include "stopwatch.hpp"
#include "rc4prng.hpp"
#include "Settings.h"
#include "testingUtils.h"

void test_group_parallel_orig(rc4prng<>& rng, const concurrency::concurrent_unordered_set<uint64_t>& cs, splayset<uint64_t>&st)
{
	uint64_t begin = rng.random(), mult = rng.random();
	for (size_t i = 0; i < group_accesses; ++i) {
		int64_t query = begin + mult * rng.random(group_size);
		auto result = st.find(query);
		auto result2 = cs.find(query);
		my_assert((result == nullptr) == (result2 == cs.end()));
		my_assert(!result || (*result2 == result->data));
	}
}

int main_parallel_orig_performance()
{
	splayset<uint64_t> st;
	Concurrency::concurrent_unordered_set<uint64_t> cs;
	/*splayset<uint64_t> st2;

	st2.insert(5);
	st2.insert(7);
	st2.insert(10);
	st2.insert(8);
	st2.insert(6);
	st2.insert(3);
	st2.insert(2);
	st2.insert(1);

	auto res = st2.find(2);
	res = st2.find(2);
	res = st2.find(2);
	res = st2.find(2);
	res = st2.find(10);

	st2.erase(2);
	st2.erase(8);
	st2.erase(7);
	res = st2.find(2);
	res = st2.find(8);
	res = st2.find(10);
	st2.erase(5);
	st2.erase(10);
	st2.erase(6);
	st2.erase(3);
	st2.erase(1);

*/

	rc4prng<> thread_rng[concurrency_];
	for (size_t tid = 0; tid < concurrency_; ++tid)
		thread_rng[tid].load_key(rand_seed, rand_seed + 10 - tid); // :]

	std::vector<std::vector<std::string>> results;
	for (size_t i = 0; i < concurrency_; i++) {
		results.push_back(std::vector<std::string>());
	}

	std::thread thread[concurrency_];
	bpp::Stopwatch stopwatch(true);

	for (size_t tid = 0; tid < concurrency_; ++tid)
		thread[tid] = std::thread([tid, &thread_rng, &st, &cs]()
	{
		for (size_t i = 0; i < item_count; ++i) {
			int64_t val = thread_rng[tid].random();
			st.insert(val);
			cs.insert(val);
		}
	});

	for (size_t tid = 0; tid < concurrency_; ++tid)
		thread[tid].join();

	stopwatch.stop();
	std::cout << stopwatch.getMiliseconds() << std::endl;
	std::cout << "Tree size:" << st.size() << std::endl;
	std::cout << "Checking tree consistency ..." << std::endl;
	testTreeConsistency(st, concurrency_ * item_count);
	std::cout << "Done" << std::endl;
	stopwatch.start();

	for (size_t tid = 0; tid < concurrency_; ++tid)
		thread[tid] = std::thread([tid, &thread_rng, &st, &cs]()
	{
		for (size_t grp = 0; grp < group_count; ++grp)
			test_group_parallel_orig(thread_rng[tid], cs, st);
	});

	for (size_t tid = 0; tid < concurrency_; ++tid)
		thread[tid].join();

	stopwatch.stop();
	std::cout << stopwatch.getMiliseconds() << std::endl;
	std::cout << "Tree size:" << st.size() << std::endl;
	std::cout << "Checking tree consistency ..." << std::endl;
	testTreeConsistency(st, concurrency_ * item_count);
	std::cout << "Done" << std::endl;
	testCSnSt(cs, st);

	return 0;
}