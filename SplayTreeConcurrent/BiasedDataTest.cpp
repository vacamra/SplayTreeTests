#include <concurrent_unordered_set.h>

#ifndef private
#define private public
#endif
#include "splayset_concurrent.hpp"
#include "stopwatch.hpp"
#include "Settings.h"
#include "rc4prng.hpp"
#include "testingUtils.h"

void test_group_biased(uint64_t start, splayset<uint64_t>& st)
{
	for (size_t i = 0; i < group_accesses; i++)
	{
		st.find(i);		
	}
}

int main_biasedData()
{
	splayset<uint64_t> st;
	Concurrency::concurrent_unordered_set<uint64_t> cs;

	std::thread thread[concurrency_];
	std::vector<std::string> result;

	bpp::Stopwatch stopwatch(true);

	for (size_t tid = 0; tid < concurrency_; ++tid)
		thread[tid] = std::thread([tid, &st, &cs]()
	{
		for (size_t i = tid * item_count; i < (tid+1) * item_count; ++i) {
			st.insert(i);
			cs.insert(i);
		}
	});

	for (size_t tid = 0; tid < concurrency_; ++tid)
		thread[tid].join();
	
	stopwatch.stop();
	std::cout << stopwatch.getMiliseconds() << std::endl;
	std::cout << "Tree size:" << st.size() << std::endl;

	stopwatch.start();

	for (size_t tid = 0; tid < concurrency_; ++tid)
		thread[tid] = std::thread([&st, &cs]()
	{
		for (size_t grp = 0; grp < group_count; ++grp)
			test_group_biased(0, st);
	});

	for (size_t tid = 0; tid < concurrency_; ++tid)
		thread[tid].join();			

	stopwatch.stop();
	std::cout << stopwatch.getMiliseconds() << std::endl;
	std::cout << "Tree size:" << st.size() << std::endl;

	testCSnSt(cs, st);

	return 0;
}