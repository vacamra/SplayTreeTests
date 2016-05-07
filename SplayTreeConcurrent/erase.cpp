#include <concurrent_unordered_set.h>
#include <concurrent_vector.h>

#ifndef private
#define private public
#endif
#include <algorithm>
#include "splayset_concurrent.hpp"
#include "stopwatch.hpp"
#include "Settings.h"
#include "rc4prng.hpp"
#include "testingUtils.h"

void test_erase_find(uint64_t start, splayset<uint64_t>& st, bool onlyOdd)
{
	for (size_t i = start; i < start + item_count; i++) {
		if (onlyOdd && i % 2 == 0) continue;
		st.erase(i);
		my_assert(st.find(i) == nullptr, "item found but it has just been erased!");
	}
}

int main_erase()
{
	splayset<uint64_t> st;

	std::thread thread[concurrency_];
	std::vector<std::string> result;

	std::cout << "Generating content..." << std::endl;

	bpp::Stopwatch stopwatch(true);

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
			st.insert(val);
		}
	});

	for (size_t tid = 0; tid < concurrency_; ++tid)
		thread[tid].join();

	stopwatch.stop();
	std::cout << stopwatch.getMiliseconds() << std::endl;
	std::cout << "Tree size:" << st.size() << std::endl;

	std::cout << "Erasing odd numbers & verifying at the same time..." << std::endl;

	stopwatch.start();

	for (size_t tid = 0; tid < concurrency_; ++tid)
		thread[tid] = std::thread([tid, &st]()
	{
		test_erase_find(tid * item_count, st, true);
	});

	for (size_t tid = 0; tid < concurrency_; ++tid)
		thread[tid].join();

	stopwatch.stop();
	std::cout << stopwatch.getMiliseconds() << std::endl;
	std::cout << "Tree size:" << st.size() << std::endl;

	std::cout << "Verifying..." << std::endl;

	stopwatch.start();

	for (size_t i = 0; i < concurrency_ * item_count; i++) {
		if (i % 2 == 0)
			my_assert(st.find(i) != nullptr, "even number not found!");
		else
			my_assert(st.find(i) == nullptr, "odd number found!");
	}

	stopwatch.stop();
	std::cout << stopwatch.getMiliseconds() << std::endl;
	std::cout << "Tree size:" << st.size() << std::endl;

	std::cout << "Erasing all numbers & verifying at the same time..." << std::endl;

	stopwatch.start();

	for (size_t tid = 0; tid < concurrency_; ++tid)
		thread[tid] = std::thread([tid, &st]()
	{
		test_erase_find(tid * item_count, st, false);
	});

	for (size_t tid = 0; tid < concurrency_; ++tid)
		thread[tid].join();

	stopwatch.stop();
	std::cout << stopwatch.getMiliseconds() << std::endl;
	std::cout << "Tree size:" << st.size() << std::endl;

	std::cout << "Verifying..." << std::endl;

	stopwatch.start();

	for (size_t i = 0; i < concurrency_ * item_count; i++) {
		my_assert(st.find(i) == nullptr, "a number has been found, but the tree should be empty!");
	}

	stopwatch.stop();
	std::cout << stopwatch.getMiliseconds() << std::endl;
	std::cout << "Tree size:" << st.size() << std::endl;

	std::cout << "[!] Do not forget to check memory consumption! Memory leaks not tested here!" << std::endl;

	return 0;
}