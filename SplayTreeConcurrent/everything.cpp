#include "Settings.h"

#include <cassert>
#include <mutex>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <cstdint>
#include <thread>
#include <unordered_set>

//Don't try this at home - for root access
#define private public 
#include "splayset_concurrent.hpp"
#include "stopwatch.hpp"
#include "rc4prng.hpp"
#include "testingUtils.h"

int main_everything()
{
	splayset<uint64_t> st;
	Concurrency::concurrent_unordered_set<uint64_t> cs;

	rc4prng<> thread_rng[concurrency_];
	for (size_t tid = 0; tid < concurrency_; ++tid)
		thread_rng[tid].load_key(rand_seed, rand_seed + 10 - tid); // :]

	std::vector<std::vector<std::string>> results;
	for (size_t i = 0; i < concurrency_; i++) {
		results.push_back(std::vector<std::string>());
	}

	my_assert(st.empty());

	std::thread thread[concurrency_];
	bpp::Stopwatch stopwatch(true);

	// generate concurrency_ * item_count random items
	std::unordered_set<int64_t> randomDataSet;
	while (randomDataSet.size() < concurrency_ * item_count)
		randomDataSet.insert(thread_rng[0].random());

	std::vector<int64_t> randomData;
	for (auto i : randomDataSet)
		randomData.emplace_back(i);

	for (size_t tid = 0; tid < concurrency_; ++tid)
		thread[tid] = std::thread([tid, &thread_rng, &st, &cs, &randomData]()
	{
		for (size_t i = tid * item_count; i < (tid + 1) * item_count; ++i) {
			int64_t val = randomData[i];
			st.insert(val);
			cs.insert(val);
			my_assert(st.find(val) != nullptr);
			if (i % 2 == 0)
			{
				st.erase(val);
				my_assert(st.find(val) == nullptr);
				st.insert(val);
				my_assert(st.find(val) != nullptr);
			}
		}
	});

	for (size_t tid = 0; tid < concurrency_; ++tid)
		thread[tid].join();

	stopwatch.stop();

	std::cout << stopwatch.getMiliseconds() << std::endl;
	std::cout << "Tree size:" << st.size() << std::endl;
	std::cout << "Checking tree consistency ..." << std::endl;
	testTreeConsistency(st, cs.size());
	std::cout << "Done" << std::endl;
	std::cout << "Checking tree content ..." << std::endl;
	testCSnSt(cs, st);
	std::cout << "Done" << std::endl;

	my_assert(!st.empty());
	my_assert(st.size() == cs.size());

	stopwatch.start();

	st.clear();
	my_assert(st.empty());
	my_assert(st.size() == 0);

	stopwatch.stop();

	std::cout << stopwatch.getMiliseconds() << std::endl;
	std::cout << "Tree size:" << st.size() << std::endl;

	stopwatch.start();

	Concurrency::concurrent_vector<uint64_t> shuffled;
	for (size_t i = 0; i < concurrency_ * item_count; ++i) {
		shuffled.push_back(i);
	}
	std::random_shuffle(shuffled.begin(), shuffled.end());

	for (size_t tid = 0; tid < concurrency_; ++tid)
		thread[tid] = std::thread([tid, &st, &shuffled]()
	{
		for (size_t i = tid * item_count; i < (tid + 1) * item_count; ++i) {
			my_assert(i < shuffled.size());
			auto val = shuffled[i];
			my_assert(st.find(val) == nullptr);
			st.insert(val);
			my_assert(st.find(val) != nullptr);
			st.erase(val);
			my_assert(st.find(val) == nullptr);
			if (val % 3 == 0)
			{
				st.insert(val);
				my_assert(st.find(val) != nullptr);
			}
		}
	});

	for (size_t tid = 0; tid < concurrency_; ++tid)
		thread[tid].join();

	stopwatch.stop();

	std::cout << stopwatch.getMiliseconds() << std::endl;
	std::cout << "Tree size:" << st.size() << std::endl;
	std::cout << "Checking tree content ..." << std::endl;
	
	size_t count = 0;
	for (size_t i = 0; i < concurrency_ * item_count; i++) {
		if (i % 3 == 0)
			my_assert(++count && st.find(i) != nullptr);
		else
			my_assert(st.find(i) == nullptr);
	}

	my_assert(!st.empty());
	my_assert(st.size() == count);

	std::cout << "Done" << std::endl;

	return 0;
}
