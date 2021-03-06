#include "Settings.h"

#ifndef private
#define private public
#endif
#include <algorithm>
#include "splayset_concurrent.hpp"
#include "stopwatch.hpp"
#include "rc4prng.hpp"
#include "testingUtils.h"

void test_nothing_is_found(uint64_t start, splayset<uint64_t>& st)
{
	for (size_t i = 0; i < group_accesses; i++) {
		my_assert(st.find(i) == nullptr, "item found but the tree should be empty");
	}
}

int main_clear()
{
	splayset<uint64_t> st;

	std::thread thread[concurrency_];
	std::vector<std::string> result;

	std::cout << "Generating content..." << std::endl;

	bpp::Stopwatch stopwatch(true);

	Concurrency::concurrent_vector<uint64_t> shuffled;
	for (size_t i = 0; i < (concurrency_ + 1) * item_count; ++i) {
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

	my_assert(!st.empty(), "empty() should not be true");

	std::cout << "Clearing..." << std::endl;

	stopwatch.start();

	st.clear();

	stopwatch.stop();
	std::cout << stopwatch.getMiliseconds() << std::endl;
	std::cout << "Tree size:" << st.size() << std::endl;

	std::cout << "Verifying tree is empty..." << std::endl;

	stopwatch.start();

	for (size_t tid = 0; tid < concurrency_; ++tid)
		thread[tid] = std::thread([&st]()
	{
		for (size_t grp = 0; grp < group_count; ++grp)
			test_nothing_is_found(0, st);
	});

	for (size_t tid = 0; tid < concurrency_; ++tid)
		thread[tid].join();

	my_assert(st.empty(), "empty() should be true");

	stopwatch.stop();
	std::cout << stopwatch.getMiliseconds() << std::endl;
	std::cout << "Tree size:" << st.size() << std::endl;

	std::cout << "[!] Do not forget to check memory consumption! Memory leaks not tested here!" << std::endl;

	return 0;
}